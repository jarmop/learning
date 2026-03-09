#include "mini_egl_internal.h"

#include <stdlib.h>

#include <EGL/egl.h>
#include <EGL/eglext.h>

struct egl_driver_data {
    EGLDisplay egl_display;
    EGLConfig egl_config;
};

struct egl_context_data {
    EGLContext egl_context;
};

struct egl_surface_data {
    EGLSurface egl_surface;
};

static int egl_init_driver(struct mini_display *dpy) {
    if (!dpy || !dpy->gbm) {
        return MINI_ERR_BAD_ARG;
    }

    struct egl_driver_data *dd = calloc(1, sizeof(*dd));
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

    static const EGLint cfg_attribs[] = {
        EGL_SURFACE_TYPE,    EGL_WINDOW_BIT,
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
        EGL_RED_SIZE,        8,
        EGL_GREEN_SIZE,      8,
        EGL_BLUE_SIZE,       8,
        EGL_NONE
    };

    EGLint num_configs = 0;
    if (!eglChooseConfig(dd->egl_display, cfg_attribs, &dd->egl_config, 1, &num_configs) ||
        num_configs < 1) {
        eglTerminate(dd->egl_display);
        free(dd);
        return MINI_ERR_BAD_STATE;
    }

    /*
     * Let the hook refine the backend's chosen format if EGL requires a different
     * native visual. This keeps the custom policy layer small but working.
     */
    EGLint visual_id = 0;
    if (eglGetConfigAttrib(dd->egl_display, dd->egl_config, EGL_NATIVE_VISUAL_ID, &visual_id)) {
        dpy->default_config.gbm_format = (uint32_t)visual_id;
        dpy->default_config.has_alpha =
            (visual_id == GBM_FORMAT_ARGB8888 || visual_id == GBM_FORMAT_ABGR8888) ? 1 : 0;
    }

    dpy->driver_data = dd;
    return MINI_OK;
}

static void egl_shutdown_driver(struct mini_display *dpy) {
    if (!dpy || !dpy->driver_data) {
        return;
    }

    struct egl_driver_data *dd = dpy->driver_data;

    eglMakeCurrent(dd->egl_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    eglTerminate(dd->egl_display);

    free(dd);
    dpy->driver_data = NULL;
}

static int egl_create_context(struct mini_context *ctx) {
    if (!ctx || !ctx->dpy || !ctx->dpy->driver_data) {
        return MINI_ERR_BAD_ARG;
    }

    struct egl_driver_data *dd = ctx->dpy->driver_data;
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

    ctx->driver_data = cd;
    return MINI_OK;
}

static void egl_destroy_context(struct mini_context *ctx) {
    if (!ctx || !ctx->dpy || !ctx->dpy->driver_data || !ctx->driver_data) {
        return;
    }

    struct egl_driver_data *dd = ctx->dpy->driver_data;
    struct egl_context_data *cd = ctx->driver_data;

    eglDestroyContext(dd->egl_display, cd->egl_context);
    free(cd);
    ctx->driver_data = NULL;
}

static int egl_create_drawable(struct mini_surface *surf) {
    if (!surf || !surf->dpy || !surf->dpy->driver_data || !surf->gbm_surface) {
        return MINI_ERR_BAD_ARG;
    }

    struct egl_driver_data *dd = surf->dpy->driver_data;
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

    surf->driver_data = sd;
    return MINI_OK;
}

static void egl_destroy_drawable(struct mini_surface *surf) {
    if (!surf || !surf->dpy || !surf->dpy->driver_data || !surf->driver_data) {
        return;
    }

    struct egl_driver_data *dd = surf->dpy->driver_data;
    struct egl_surface_data *sd = surf->driver_data;

    eglDestroySurface(dd->egl_display, sd->egl_surface);
    free(sd);
    surf->driver_data = NULL;
}

static int egl_bind_current(
    struct mini_display *dpy,
    struct mini_surface *draw,
    struct mini_surface *read,
    struct mini_context *ctx
) {
    if (!dpy || !draw || !read || !ctx || !dpy->driver_data) {
        return MINI_ERR_BAD_ARG;
    }

    if (!draw->driver_data || !read->driver_data || !ctx->driver_data) {
        return MINI_ERR_BAD_STATE;
    }

    struct egl_driver_data *dd = dpy->driver_data;
    struct egl_surface_data *draw_sd = draw->driver_data;
    struct egl_surface_data *read_sd = read->driver_data;
    struct egl_context_data *cd = ctx->driver_data;

    if (!eglMakeCurrent(
            dd->egl_display,
            draw_sd->egl_surface,
            read_sd->egl_surface,
            cd->egl_context)) {
        return MINI_ERR_BAD_STATE;
    }

    return MINI_OK;
}

static int egl_unbind_current(struct mini_display *dpy) {
    if (!dpy || !dpy->driver_data) {
        return MINI_ERR_BAD_ARG;
    }

    struct egl_driver_data *dd = dpy->driver_data;

    if (!eglMakeCurrent(dd->egl_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT)) {
        return MINI_ERR_BAD_STATE;
    }

    return MINI_OK;
}

static int egl_flush_and_publish(struct mini_display *dpy, struct mini_surface *surf) {
    if (!dpy || !surf || !dpy->driver_data || !surf->driver_data) {
        return MINI_ERR_BAD_ARG;
    }

    struct egl_driver_data *dd = dpy->driver_data;
    struct egl_surface_data *sd = surf->driver_data;

    if (!eglSwapBuffers(dd->egl_display, sd->egl_surface)) {
        return MINI_ERR_BAD_STATE;
    }

    return MINI_OK;
}

static const struct mini_driver_hook_vtbl egl_hook_vtbl = {
    .init_driver = egl_init_driver,
    .shutdown_driver = egl_shutdown_driver,
    .create_context = egl_create_context,
    .destroy_context = egl_destroy_context,
    .create_drawable = egl_create_drawable,
    .destroy_drawable = egl_destroy_drawable,
    .bind_current = egl_bind_current,
    .unbind_current = egl_unbind_current,
    .flush_and_publish = egl_flush_and_publish,
};

const struct mini_driver_hook mini_driver_hook_egl = {
    .name = "egl-hook",
    .vtbl = &egl_hook_vtbl,
};