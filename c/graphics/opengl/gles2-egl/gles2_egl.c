#define _GNU_SOURCE
#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <xf86drm.h>
#include <xf86drmMode.h>
#include <gbm.h>

#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES2/gl2.h>

struct drm_fb {
    struct gbm_bo *bo;
    uint32_t fb_id;
};

struct app {
    int drm_fd;

    drmModeModeInfo mode;
    uint32_t conn_id;
    uint32_t crtc_id;
    drmModeCrtc *old_crtc;

    struct gbm_device *gbm;
    struct gbm_surface *gbm_surface;

    EGLDisplay egl_display;
    EGLContext egl_context;
    EGLSurface egl_surface;
    EGLConfig egl_config;

    struct gbm_bo *front_bo;
    uint32_t front_fb_id;
};

static void fail(const char *msg) {
    fprintf(stderr, "%s: %s\n", msg, strerror(errno));
    exit(1);
}

static void fail_egl(const char *msg) {
    fprintf(stderr, "%s: EGL error 0x%04x\n", msg, eglGetError());
    exit(1);
}

static uint32_t bo_get_fb_id(struct gbm_bo *bo) {
    void *ptr = gbm_bo_get_user_data(bo);
    if (ptr) {
        return (uint32_t)(uintptr_t)ptr;
    }

    uint32_t width  = gbm_bo_get_width(bo);
    uint32_t height = gbm_bo_get_height(bo);
    uint32_t stride = gbm_bo_get_stride(bo);
    uint32_t handle = gbm_bo_get_handle(bo).u32;
    uint32_t fb_id  = 0;

    int drm_fd = gbm_device_get_fd(gbm_bo_get_device(bo));

    if (drmModeAddFB(drm_fd, width, height, 24, 32, stride, handle, &fb_id) != 0) {
        fail("drmModeAddFB");
    }

    gbm_bo_set_user_data(bo, (void *)(uintptr_t)fb_id, NULL);
    return fb_id;
}

static void pick_connector_and_crtc(struct app *a) {
    drmModeRes *res = drmModeGetResources(a->drm_fd);
    if (!res) fail("drmModeGetResources");

    drmModeConnector *chosen_conn = NULL;
    drmModeEncoder *chosen_enc = NULL;

    for (int i = 0; i < res->count_connectors; i++) {
        drmModeConnector *conn = drmModeGetConnector(a->drm_fd, res->connectors[i]);
        if (!conn) continue;

        if (conn->connection == DRM_MODE_CONNECTED && conn->count_modes > 0) {
            chosen_conn = conn;
            break;
        }

        drmModeFreeConnector(conn);
    }

    if (!chosen_conn) {
        fprintf(stderr, "No connected connector found\n");
        exit(1);
    }

    a->conn_id = chosen_conn->connector_id;
    a->mode = chosen_conn->modes[0]; /* first advertised mode */

    if (chosen_conn->encoder_id) {
        chosen_enc = drmModeGetEncoder(a->drm_fd, chosen_conn->encoder_id);
    }

    if (!chosen_enc) {
        fprintf(stderr, "No encoder found for connector\n");
        exit(1);
    }

    a->crtc_id = chosen_enc->crtc_id;
    a->old_crtc = drmModeGetCrtc(a->drm_fd, a->crtc_id);
    if (!a->old_crtc) fail("drmModeGetCrtc");

    drmModeFreeEncoder(chosen_enc);
    drmModeFreeConnector(chosen_conn);
    drmModeFreeResources(res);
}

static void init_gbm_device(struct app *a) {
    a->gbm = gbm_create_device(a->drm_fd);
    if (!a->gbm) {
        fprintf(stderr, "gbm_create_device failed\n");
        exit(1);
    }
}

static void init_egl(struct app *a) {
    PFNEGLGETPLATFORMDISPLAYPROC get_platform_display =
        (PFNEGLGETPLATFORMDISPLAYPROC)eglGetProcAddress("eglGetPlatformDisplay");

    if (get_platform_display) {
        a->egl_display = get_platform_display(EGL_PLATFORM_GBM_KHR, a->gbm, NULL);
    } else {
        a->egl_display = eglGetDisplay((EGLNativeDisplayType)a->gbm);
    }

    if (a->egl_display == EGL_NO_DISPLAY) fail_egl("eglGetDisplay");

    if (!eglInitialize(a->egl_display, NULL, NULL)) fail_egl("eglInitialize");
    if (!eglBindAPI(EGL_OPENGL_ES_API)) fail_egl("eglBindAPI");

    static const EGLint cfg_attribs[] = {
        EGL_SURFACE_TYPE,    EGL_WINDOW_BIT,
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
        EGL_RED_SIZE,        8,
        EGL_GREEN_SIZE,      8,
        EGL_BLUE_SIZE,       8,
        EGL_NONE
    };

    EGLint num_configs = 0;
    if (!eglChooseConfig(a->egl_display, cfg_attribs, &a->egl_config, 1, &num_configs) ||
        num_configs < 1) {
        fail_egl("eglChooseConfig");
    }

    EGLint visual_id = 0;
    if (!eglGetConfigAttrib(a->egl_display, a->egl_config, EGL_NATIVE_VISUAL_ID, &visual_id)) {
        fail_egl("eglGetConfigAttrib(EGL_NATIVE_VISUAL_ID)");
    }

    printf("Using GBM format 0x%x\n", visual_id);

    a->gbm_surface = gbm_surface_create(
        a->gbm,
        a->mode.hdisplay,
        a->mode.vdisplay,
        (uint32_t)visual_id,
        GBM_BO_USE_SCANOUT | GBM_BO_USE_RENDERING
    );
    if (!a->gbm_surface) {
        fprintf(stderr, "gbm_surface_create failed\n");
        exit(1);
    }

    static const EGLint ctx_attribs[] = {
        EGL_CONTEXT_CLIENT_VERSION, 2,
        EGL_NONE
    };

    a->egl_context = eglCreateContext(
        a->egl_display, a->egl_config, EGL_NO_CONTEXT, ctx_attribs
    );
    if (a->egl_context == EGL_NO_CONTEXT) fail_egl("eglCreateContext");

    PFNEGLCREATEPLATFORMWINDOWSURFACEPROC create_platform_window_surface =
        (PFNEGLCREATEPLATFORMWINDOWSURFACEPROC)
            eglGetProcAddress("eglCreatePlatformWindowSurface");

    if (create_platform_window_surface) {
        a->egl_surface = create_platform_window_surface(
            a->egl_display, a->egl_config, (void *)a->gbm_surface, NULL
        );
    } else {
        a->egl_surface = eglCreateWindowSurface(
            a->egl_display, a->egl_config, (EGLNativeWindowType)a->gbm_surface, NULL
        );
    }

    if (a->egl_surface == EGL_NO_SURFACE) {
        fail_egl("eglCreatePlatformWindowSurface/eglCreateWindowSurface");
    }

    if (!eglMakeCurrent(a->egl_display, a->egl_surface, a->egl_surface, a->egl_context)) {
        fail_egl("eglMakeCurrent");
    }
}

static void draw_and_show(struct app *a) {
    glViewport(0, 0, a->mode.hdisplay, a->mode.vdisplay);
    glClearColor(0.1f, 0.2f, 0.7f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    if (!eglSwapBuffers(a->egl_display, a->egl_surface)) {
        fail_egl("eglSwapBuffers");
    }

    a->front_bo = gbm_surface_lock_front_buffer(a->gbm_surface);
    if (!a->front_bo) {
        fprintf(stderr, "gbm_surface_lock_front_buffer failed\n");
        exit(1);
    }

    a->front_fb_id = bo_get_fb_id(a->front_bo);

    if (drmModeSetCrtc(
            a->drm_fd,
            a->crtc_id,
            a->front_fb_id,
            0, 0,
            &a->conn_id, 1,
            &a->mode) != 0) {
        fail("drmModeSetCrtc");
    }
}

static void cleanup(struct app *a) {
    if (a->old_crtc) {
        drmModeSetCrtc(
            a->drm_fd,
            a->old_crtc->crtc_id,
            a->old_crtc->buffer_id,
            a->old_crtc->x,
            a->old_crtc->y,
            &a->conn_id,
            1,
            &a->old_crtc->mode
        );
    }

    if (a->front_bo) {
        gbm_surface_release_buffer(a->gbm_surface, a->front_bo);
    }

    if (a->egl_display != EGL_NO_DISPLAY) {
        eglMakeCurrent(a->egl_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        if (a->egl_surface != EGL_NO_SURFACE) {
            eglDestroySurface(a->egl_display, a->egl_surface);
        }
        if (a->egl_context != EGL_NO_CONTEXT) {
            eglDestroyContext(a->egl_display, a->egl_context);
        }
        eglTerminate(a->egl_display);
    }

    if (a->gbm_surface) gbm_surface_destroy(a->gbm_surface);
    if (a->gbm) gbm_device_destroy(a->gbm);

    if (a->old_crtc) drmModeFreeCrtc(a->old_crtc);
    if (a->drm_fd >= 0) close(a->drm_fd);
}

int main(void) {
    struct app a = {
        .drm_fd = -1,
        .egl_display = EGL_NO_DISPLAY,
        .egl_context = EGL_NO_CONTEXT,
        .egl_surface = EGL_NO_SURFACE,
    };

    char *drm_device = "/dev/dri/card1";
    a.drm_fd = open(drm_device, O_RDWR | O_CLOEXEC);
    if (a.drm_fd < 0) fail(drm_device);

    pick_connector_and_crtc(&a);
    init_gbm_device(&a);
    init_egl(&a);
    draw_and_show(&a);

    sleep(5);

    cleanup(&a);
    return 0;
}