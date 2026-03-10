#include "mini_egl_internal.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dlfcn.h>

#include <EGL/egl.h>
#include <EGL/eglext.h>

struct egl_driver_data {
    void *libegl;

    EGLDisplay egl_display;
    EGLConfig egl_config;

    /* function pointers */
    EGLDisplay (*p_eglGetDisplay)(EGLNativeDisplayType display_id);
    EGLBoolean (*p_eglInitialize)(EGLDisplay dpy, EGLint *major, EGLint *minor);
    EGLBoolean (*p_eglBindAPI)(EGLenum api);
    EGLBoolean (*p_eglChooseConfig)(EGLDisplay dpy, const EGLint *attrib_list,
                                    EGLConfig *configs, EGLint config_size, EGLint *num_config);
    EGLBoolean (*p_eglGetConfigAttrib)(EGLDisplay dpy, EGLConfig config,
                                       EGLint attribute, EGLint *value);
    EGLContext (*p_eglCreateContext)(EGLDisplay dpy, EGLConfig config,
                                     EGLContext share_context, const EGLint *attrib_list);
    EGLSurface (*p_eglCreateWindowSurface)(EGLDisplay dpy, EGLConfig config,
                                           EGLNativeWindowType win, const EGLint *attrib_list);
    EGLBoolean (*p_eglDestroyContext)(EGLDisplay dpy, EGLContext ctx);
    EGLBoolean (*p_eglDestroySurface)(EGLDisplay dpy, EGLSurface surface);
    EGLBoolean (*p_eglMakeCurrent)(EGLDisplay dpy, EGLSurface draw,
                                   EGLSurface read, EGLContext ctx);
    EGLBoolean (*p_eglSwapBuffers)(EGLDisplay dpy, EGLSurface surface);
    EGLBoolean (*p_eglTerminate)(EGLDisplay dpy);
    EGLint (*p_eglGetError)(void);
    __eglMustCastToProperFunctionPointerType (*p_eglGetProcAddress)(const char *procname);

    PFNEGLGETPLATFORMDISPLAYPROC p_eglGetPlatformDisplay;
    PFNEGLCREATEPLATFORMWINDOWSURFACEPROC p_eglCreatePlatformWindowSurface;
};

struct egl_context_data {
    EGLContext egl_context;
};

struct egl_surface_data {
    EGLSurface egl_surface;
};

static int load_required_symbol(void *lib, const char *name, void **out)
{
    *out = dlsym(lib, name);
    return *out ? MINI_OK : MINI_ERR_BAD_STATE;
}

static int egl_load_symbols(struct egl_driver_data *dd)
{
    dd->libegl = dlopen("libEGL.so.1", RTLD_NOW | RTLD_LOCAL);
    if (!dd->libegl) {
        return MINI_ERR_BAD_STATE;
    }

    if (load_required_symbol(dd->libegl, "eglGetDisplay", (void **)&dd->p_eglGetDisplay) != MINI_OK ||
        load_required_symbol(dd->libegl, "eglInitialize", (void **)&dd->p_eglInitialize) != MINI_OK ||
        load_required_symbol(dd->libegl, "eglBindAPI", (void **)&dd->p_eglBindAPI) != MINI_OK ||
        load_required_symbol(dd->libegl, "eglChooseConfig", (void **)&dd->p_eglChooseConfig) != MINI_OK ||
        load_required_symbol(dd->libegl, "eglGetConfigAttrib", (void **)&dd->p_eglGetConfigAttrib) != MINI_OK ||
        load_required_symbol(dd->libegl, "eglCreateContext", (void **)&dd->p_eglCreateContext) != MINI_OK ||
        load_required_symbol(dd->libegl, "eglCreateWindowSurface", (void **)&dd->p_eglCreateWindowSurface) != MINI_OK ||
        load_required_symbol(dd->libegl, "eglDestroyContext", (void **)&dd->p_eglDestroyContext) != MINI_OK ||
        load_required_symbol(dd->libegl, "eglDestroySurface", (void **)&dd->p_eglDestroySurface) != MINI_OK ||
        load_required_symbol(dd->libegl, "eglMakeCurrent", (void **)&dd->p_eglMakeCurrent) != MINI_OK ||
        load_required_symbol(dd->libegl, "eglSwapBuffers", (void **)&dd->p_eglSwapBuffers) != MINI_OK ||
        load_required_symbol(dd->libegl, "eglTerminate", (void **)&dd->p_eglTerminate) != MINI_OK ||
        load_required_symbol(dd->libegl, "eglGetError", (void **)&dd->p_eglGetError) != MINI_OK ||
        load_required_symbol(dd->libegl, "eglGetProcAddress", (void **)&dd->p_eglGetProcAddress) != MINI_OK) {
        dlclose(dd->libegl);
        dd->libegl = NULL;
        return MINI_ERR_BAD_STATE;
    }

    dd->p_eglGetPlatformDisplay =
        (PFNEGLGETPLATFORMDISPLAYPROC)dd->p_eglGetProcAddress("eglGetPlatformDisplay");
    dd->p_eglCreatePlatformWindowSurface =
        (PFNEGLCREATEPLATFORMWINDOWSURFACEPROC)dd->p_eglGetProcAddress("eglCreatePlatformWindowSurface");

    return MINI_OK;
}

static int egl_init_driver(struct mini_display *dpy) {
    if (!dpy || !dpy->gbm) {
        return MINI_ERR_BAD_ARG;
    }

    struct egl_driver_data *dd = calloc(1, sizeof(*dd));
    if (!dd) {
        return MINI_ERR_NOMEM;
    }

    int rc = egl_load_symbols(dd);
    if (rc != MINI_OK) {
        free(dd);
        return rc;
    }

    if (dd->p_eglGetPlatformDisplay) {
        dd->egl_display = dd->p_eglGetPlatformDisplay(EGL_PLATFORM_GBM_KHR, dpy->gbm, NULL);
    } else {
        dd->egl_display = dd->p_eglGetDisplay((EGLNativeDisplayType)dpy->gbm);
    }

    if (dd->egl_display == EGL_NO_DISPLAY) {
        dlclose(dd->libegl);
        free(dd);
        return MINI_ERR_BAD_STATE;
    }

    if (!dd->p_eglInitialize(dd->egl_display, NULL, NULL)) {
        dd->p_eglTerminate(dd->egl_display);
        dlclose(dd->libegl);
        free(dd);
        return MINI_ERR_BAD_STATE;
    }

    if (!dd->p_eglBindAPI(EGL_OPENGL_ES_API)) {
        dd->p_eglTerminate(dd->egl_display);
        dlclose(dd->libegl);
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
    if (!dd->p_eglChooseConfig(dd->egl_display, cfg_attribs, &dd->egl_config, 1, &num_configs) ||
        num_configs < 1) {
        dd->p_eglTerminate(dd->egl_display);
        dlclose(dd->libegl);
        free(dd);
        return MINI_ERR_BAD_STATE;
    }

    dpy->driver_data = dd;
    return MINI_OK;
}

static void egl_shutdown_driver(struct mini_display *dpy) {
    if (!dpy || !dpy->driver_data) {
        return;
    }

    struct egl_driver_data *dd = dpy->driver_data;

    dd->p_eglMakeCurrent(dd->egl_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    dd->p_eglTerminate(dd->egl_display);
    if (dd->libegl) {
        dlclose(dd->libegl);
    }

    free(dd);
    dpy->driver_data = NULL;
}

static int egl_query_native_format(struct mini_display *dpy, uint32_t *out_format) {
    if (!dpy || !dpy->driver_data || !out_format) {
        return MINI_ERR_BAD_ARG;
    }

    struct egl_driver_data *dd = dpy->driver_data;

    EGLint visual_id = 0;
    if (!dd->p_eglGetConfigAttrib(dd->egl_display, dd->egl_config, EGL_NATIVE_VISUAL_ID, &visual_id)) {
        return MINI_ERR_BAD_STATE;
    }

    *out_format = (uint32_t)visual_id;
    return MINI_OK;
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

    cd->egl_context = dd->p_eglCreateContext(
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

    dd->p_eglDestroyContext(dd->egl_display, cd->egl_context);
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

    if (dd->p_eglCreatePlatformWindowSurface) {
        sd->egl_surface = dd->p_eglCreatePlatformWindowSurface(
            dd->egl_display,
            dd->egl_config,
            (void *)surf->gbm_surface,
            NULL
        );
    } else {
        sd->egl_surface = dd->p_eglCreateWindowSurface(
            dd->egl_display,
            dd->egl_config,
            (EGLNativeWindowType)surf->gbm_surface,
            NULL
        );
    }

    if (sd->egl_surface == EGL_NO_SURFACE) {
        fprintf(stderr, "egl_create_drawable failed: EGL error 0x%x\n", dd->p_eglGetError());
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

    dd->p_eglDestroySurface(dd->egl_display, sd->egl_surface);
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

    if (!dd->p_eglMakeCurrent(
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

    if (!dd->p_eglMakeCurrent(dd->egl_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT)) {
        return MINI_ERR_BAD_STATE;
    }

    return MINI_OK;
}

static int egl_flush_rendering(struct mini_display *dpy, struct mini_surface *surf) {
    (void)dpy;
    (void)surf;
    return MINI_OK;
}

static int egl_publish_surface(struct mini_display *dpy, struct mini_surface *surf) {
    if (!dpy || !surf || !dpy->driver_data || !surf->driver_data) {
        return MINI_ERR_BAD_ARG;
    }

    struct egl_driver_data *dd = dpy->driver_data;
    struct egl_surface_data *sd = surf->driver_data;

    if (!dd->p_eglSwapBuffers(dd->egl_display, sd->egl_surface)) {
        fprintf(stderr, "egl_publish_surface failed: EGL error 0x%x\n", dd->p_eglGetError());
        return MINI_ERR_BAD_STATE;
    }

    return MINI_OK;
}

static int egl_acquire_front_buffer(struct mini_surface *surf, struct gbm_bo **out_bo) {
    if (!surf || !surf->gbm_surface || !out_bo) {
        return MINI_ERR_BAD_ARG;
    }

    struct gbm_bo *bo = gbm_surface_lock_front_buffer(surf->gbm_surface);
    if (!bo) {
        return MINI_ERR_BAD_STATE;
    }

    *out_bo = bo;
    return MINI_OK;
}

static void egl_release_front_buffer(struct mini_surface *surf, struct gbm_bo *bo) {
    if (!surf || !surf->gbm_surface || !bo) {
        return;
    }

    gbm_surface_release_buffer(surf->gbm_surface, bo);
}

static const struct mini_driver_hook_vtbl egl_hook_vtbl = {
    .init_driver = egl_init_driver,
    .shutdown_driver = egl_shutdown_driver,
    .query_native_format = egl_query_native_format,
    .create_context = egl_create_context,
    .destroy_context = egl_destroy_context,
    .create_drawable = egl_create_drawable,
    .destroy_drawable = egl_destroy_drawable,
    .bind_current = egl_bind_current,
    .unbind_current = egl_unbind_current,
    .flush_rendering = egl_flush_rendering,
    .publish_surface = egl_publish_surface,
    .acquire_front_buffer = egl_acquire_front_buffer,
    .release_front_buffer = egl_release_front_buffer,
};

const struct mini_driver_hook mini_driver_hook_egl = {
    .name = "egl-hook-dlopen",
    .vtbl = &egl_hook_vtbl,
};