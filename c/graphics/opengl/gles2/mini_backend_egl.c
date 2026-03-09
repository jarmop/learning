#include "mini_egl_internal.h"

#include <stdlib.h>
#include <string.h>

#include <EGL/egl.h>
#include <EGL/eglext.h>

struct egl_display_data {
    EGLDisplay egl_display;
    EGLConfig egl_config;
};

struct egl_context_data {
    EGLContext egl_context;
};

struct egl_surface_data {
    EGLSurface egl_surface;
};

static int egl_choose_config(struct mini_display *dpy, EGLDisplay edpy, EGLConfig *out_cfg) {
    (void)dpy;

    static const EGLint cfg_attribs[] = {
        EGL_SURFACE_TYPE,    EGL_WINDOW_BIT,
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
        EGL_RED_SIZE,        8,
        EGL_GREEN_SIZE,      8,
        EGL_BLUE_SIZE,       8,
        EGL_NONE
    };

    EGLint num_configs = 0;
    if (!eglChooseConfig(edpy, cfg_attribs, out_cfg, 1, &num_configs)) {
        return MINI_ERR_BAD_STATE;
    }

    if (num_configs < 1) {
        return MINI_ERR_UNSUPPORTED;
    }

    return MINI_OK;
}

static int egl_backend_initialize(struct mini_display *dpy) {
    if (!dpy || !dpy->gbm) {
        return MINI_ERR_BAD_ARG;
    }

    struct egl_display_data *dd = calloc(1, sizeof(*dd));
    if (!dd) {
        return MINI_ERR_NOMEM;
    }

    PFNEGLGETPLATFORMDISPLAYPROC get_platform_display =
        (PFNEGLGETPLATFORMDISPLAYPROC)eglGetProcAddress("eglGetPlatformDisplay");

    if (get_platform_display) {
        dd->egl_display = get_platform_display(EGL_PLATFORM_GBM_KHR, dpy->gbm, NULL);
    } else {
        dd->egl_display = eglGetDisplay((EGLNativeDisplayType)dpy->gbm);
    }

    if (dd->egl_display == EGL_NO_DISPLAY) {
        free(dd);
        return MINI_ERR_BAD_STATE;
    }

    if (!eglInitialize(dd->egl_display, NULL, NULL)) {
        free(dd);
        return MINI_ERR_BAD_STATE;
    }

    if (!eglBindAPI(EGL_OPENGL_ES_API)) {
        eglTerminate(dd->egl_display);
        free(dd);
        return MINI_ERR_BAD_STATE;
    }

    int rc = egl_choose_config(dpy, dd->egl_display, &dd->egl_config);
    if (rc != MINI_OK) {
        eglTerminate(dd->egl_display);
        free(dd);
        return rc;
    }

    /*
     * Match the display's default config to the actual EGL config.
     * This keeps the mini-EGL config aligned with the native visual format.
     */
    EGLint visual_id = 0;
    if (!eglGetConfigAttrib(dd->egl_display, dd->egl_config, EGL_NATIVE_VISUAL_ID, &visual_id)) {
        eglTerminate(dd->egl_display);
        free(dd);
        return MINI_ERR_BAD_STATE;
    }

    dpy->default_config.gbm_format = (uint32_t)visual_id;
    dpy->default_config.gles_version = 2;
    dpy->default_config.depth_bits = 0;
    dpy->default_config.stencil_bits = 0;
    dpy->default_config.has_alpha =
        (visual_id == GBM_FORMAT_ARGB8888 || visual_id == GBM_FORMAT_ABGR8888) ? 1 : 0;

    dpy->backend_data = dd;
    return MINI_OK;
}

static void egl_backend_terminate(struct mini_display *dpy) {
    if (!dpy || !dpy->backend_data) {
        return;
    }

    struct egl_display_data *dd = dpy->backend_data;

    eglMakeCurrent(dd->egl_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    eglTerminate(dd->egl_display);

    free(dd);
    dpy->backend_data = NULL;
}

static int egl_backend_create_context(struct mini_context *ctx) {
    if (!ctx || !ctx->dpy || !ctx->dpy->backend_data) {
        return MINI_ERR_BAD_ARG;
    }

    struct egl_display_data *dd = ctx->dpy->backend_data;

    struct egl_context_data *cd = calloc(1, sizeof(*cd));
    if (!cd) {
        return MINI_ERR_NOMEM;
    }

    static const EGLint ctx_attribs[] = {
        EGL_CONTEXT_CLIENT_VERSION, 2,
        EGL_NONE
    };

    cd->egl_context = eglCreateContext(
        dd->egl_display,
        dd->egl_config,
        EGL_NO_CONTEXT,
        ctx_attribs
    );

    if (cd->egl_context == EGL_NO_CONTEXT) {
        free(cd);
        return MINI_ERR_BAD_STATE;
    }

    ctx->backend_data = cd;
    return MINI_OK;
}

static void egl_backend_destroy_context(struct mini_context *ctx) {
    if (!ctx || !ctx->dpy || !ctx->dpy->backend_data || !ctx->backend_data) {
        return;
    }

    struct egl_display_data *dd = ctx->dpy->backend_data;
    struct egl_context_data *cd = ctx->backend_data;

    eglDestroyContext(dd->egl_display, cd->egl_context);
    free(cd);
    ctx->backend_data = NULL;
}

static int egl_backend_create_surface(struct mini_surface *surf) {
    if (!surf || !surf->dpy || !surf->dpy->backend_data || !surf->gbm_surface) {
        return MINI_ERR_BAD_ARG;
    }

    struct egl_display_data *dd = surf->dpy->backend_data;

    struct egl_surface_data *sd = calloc(1, sizeof(*sd));
    if (!sd) {
        return MINI_ERR_NOMEM;
    }

    PFNEGLCREATEPLATFORMWINDOWSURFACEPROC create_platform_window_surface =
        (PFNEGLCREATEPLATFORMWINDOWSURFACEPROC)
            eglGetProcAddress("eglCreatePlatformWindowSurface");

    if (create_platform_window_surface) {
        sd->egl_surface = create_platform_window_surface(
            dd->egl_display,
            dd->egl_config,
            (void *)surf->gbm_surface,
            NULL
        );
    } else {
        sd->egl_surface = eglCreateWindowSurface(
            dd->egl_display,
            dd->egl_config,
            (EGLNativeWindowType)surf->gbm_surface,
            NULL
        );
    }

    if (sd->egl_surface == EGL_NO_SURFACE) {
        free(sd);
        return MINI_ERR_BAD_STATE;
    }

    surf->backend_data = sd;
    surf->front_bo = NULL;
    surf->swapped = 0;

    return MINI_OK;
}

static void egl_backend_destroy_surface(struct mini_surface *surf) {
    if (!surf || !surf->dpy || !surf->dpy->backend_data || !surf->backend_data) {
        return;
    }

    struct egl_display_data *dd = surf->dpy->backend_data;
    struct egl_surface_data *sd = surf->backend_data;

    eglDestroySurface(dd->egl_display, sd->egl_surface);
    free(sd);
    surf->backend_data = NULL;
}

static int egl_backend_make_current(
    struct mini_display *dpy,
    struct mini_surface *draw,
    struct mini_surface *read,
    struct mini_context *ctx
) {
    if (!dpy || !dpy->backend_data) {
        return MINI_ERR_BAD_ARG;
    }

    struct egl_display_data *dd = dpy->backend_data;

    if (!draw && !read && !ctx) {
        if (!eglMakeCurrent(dd->egl_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT)) {
            return MINI_ERR_BAD_STATE;
        }
        return MINI_OK;
    }

    if (!draw || !read || !ctx) {
        return MINI_ERR_BAD_ARG;
    }

    if (draw->dpy != dpy || read->dpy != dpy || ctx->dpy != dpy) {
        return MINI_ERR_BAD_ARG;
    }

    if (!draw->backend_data || !read->backend_data || !ctx->backend_data) {
        return MINI_ERR_BAD_STATE;
    }

    struct egl_surface_data *draw_sd = draw->backend_data;
    struct egl_surface_data *read_sd = read->backend_data;
    struct egl_context_data *cd = ctx->backend_data;

    if (!eglMakeCurrent(
            dd->egl_display,
            draw_sd->egl_surface,
            read_sd->egl_surface,
            cd->egl_context)) {
        return MINI_ERR_BAD_STATE;
    }

    return MINI_OK;
}

static int egl_backend_swap_buffers(struct mini_display *dpy, struct mini_surface *surf) {
    if (!dpy || !surf || !dpy->backend_data || !surf->backend_data) {
        return MINI_ERR_BAD_ARG;
    }

    if (g_mini_current.dpy != dpy || g_mini_current.draw != surf || !g_mini_current.ctx) {
        return MINI_ERR_BAD_STATE;
    }

    struct egl_display_data *dd = dpy->backend_data;
    struct egl_surface_data *sd = surf->backend_data;

    if (!eglSwapBuffers(dd->egl_display, sd->egl_surface)) {
        return MINI_ERR_BAD_STATE;
    }

    surf->swapped = 1;
    return MINI_OK;
}

static const struct mini_backend_vtbl egl_vtbl = {
    .initialize = egl_backend_initialize,
    .terminate = egl_backend_terminate,
    .create_context = egl_backend_create_context,
    .destroy_context = egl_backend_destroy_context,
    .create_surface = egl_backend_create_surface,
    .destroy_surface = egl_backend_destroy_surface,
    .make_current = egl_backend_make_current,
    .swap_buffers = egl_backend_swap_buffers,
};

const struct mini_backend mini_backend_egl = {
    .name = "egl",
    .vtbl = &egl_vtbl,
};