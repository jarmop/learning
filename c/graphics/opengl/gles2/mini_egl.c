#include "mini_egl_internal.h"

#include <stdlib.h>

struct mini_current g_mini_current = {0};

struct mini_display *miniGetDisplay(struct gbm_device *gbm, int drm_fd) {
    if (!gbm || drm_fd < 0) {
        return NULL;
    }

    struct mini_display *dpy = calloc(1, sizeof(*dpy));
    if (!dpy) {
        return NULL;
    }

    dpy->gbm = gbm;
    dpy->drm_fd = drm_fd;
    dpy->backend = &mini_backend_egl;

    dpy->default_config.gbm_format   = GBM_FORMAT_XRGB8888;
    dpy->default_config.gles_version = 2;
    dpy->default_config.depth_bits   = 0;
    dpy->default_config.stencil_bits = 0;
    dpy->default_config.has_alpha    = 0;

    return dpy;
}

int miniInitialize(struct mini_display *dpy) {
    if (!dpy) {
        return MINI_ERR_BAD_ARG;
    }
    if (dpy->initialized) {
        return MINI_OK;
    }

    int rc = dpy->backend->vtbl->initialize(dpy);
    if (rc != MINI_OK) {
        return rc;
    }

    dpy->initialized = 1;
    return MINI_OK;
}

void miniTerminate(struct mini_display *dpy) {
    if (!dpy) {
        return;
    }

    if (dpy->initialized && dpy->backend && dpy->backend->vtbl->terminate) {
        dpy->backend->vtbl->terminate(dpy);
    }

    if (g_mini_current.dpy == dpy) {
        g_mini_current.dpy = NULL;
        g_mini_current.ctx = NULL;
        g_mini_current.draw = NULL;
        g_mini_current.read = NULL;
    }

    free(dpy);
}

const struct mini_config *miniChooseConfig(struct mini_display *dpy) {
    if (!dpy || !dpy->initialized) {
        return NULL;
    }
    return &dpy->default_config;
}

struct mini_context *miniCreateContext(
    struct mini_display *dpy,
    const struct mini_config *cfg
) {
    if (!dpy || !cfg || !dpy->initialized) {
        return NULL;
    }

    struct mini_context *ctx = calloc(1, sizeof(*ctx));
    if (!ctx) {
        return NULL;
    }

    ctx->dpy = dpy;
    ctx->cfg = *cfg;

    if (dpy->backend->vtbl->create_context(ctx) != MINI_OK) {
        free(ctx);
        return NULL;
    }

    return ctx;
}

void miniDestroyContext(struct mini_context *ctx) {
    if (!ctx) {
        return;
    }

    if (ctx->dpy && ctx->dpy->backend && ctx->dpy->backend->vtbl->destroy_context) {
        ctx->dpy->backend->vtbl->destroy_context(ctx);
    }

    if (g_mini_current.ctx == ctx) {
        g_mini_current.ctx = NULL;
        g_mini_current.draw = NULL;
        g_mini_current.read = NULL;
        g_mini_current.dpy = NULL;
    }

    free(ctx);
}

struct mini_surface *miniCreateWindowSurface(
    struct mini_display *dpy,
    const struct mini_config *cfg,
    struct gbm_surface *gbm_surface,
    int width,
    int height
) {
    if (!dpy || !cfg || !gbm_surface || width <= 0 || height <= 0 || !dpy->initialized) {
        return NULL;
    }

    struct mini_surface *surf = calloc(1, sizeof(*surf));
    if (!surf) {
        return NULL;
    }

    surf->dpy = dpy;
    surf->cfg = *cfg;
    surf->gbm_surface = gbm_surface;
    surf->width = width;
    surf->height = height;
    surf->format = cfg->gbm_format;

    if (dpy->backend->vtbl->create_surface(surf) != MINI_OK) {
        free(surf);
        return NULL;
    }

    return surf;
}

void miniDestroySurface(struct mini_surface *surf) {
    if (!surf) {
        return;
    }

    if (surf->dpy && surf->dpy->backend && surf->dpy->backend->vtbl->destroy_surface) {
        surf->dpy->backend->vtbl->destroy_surface(surf);
    }

    if (g_mini_current.draw == surf) {
        g_mini_current.draw = NULL;
    }
    if (g_mini_current.read == surf) {
        g_mini_current.read = NULL;
    }

    free(surf);
}

int miniMakeCurrent(
    struct mini_display *dpy,
    struct mini_surface *draw,
    struct mini_surface *read,
    struct mini_context *ctx
) {
    if (!dpy) {
        return MINI_ERR_BAD_ARG;
    }

    int rc = dpy->backend->vtbl->make_current(dpy, draw, read, ctx);
    if (rc != MINI_OK) {
        return rc;
    }

    g_mini_current.dpy = dpy;
    g_mini_current.ctx = ctx;
    g_mini_current.draw = draw;
    g_mini_current.read = read;
    return MINI_OK;
}

int miniSwapBuffers(struct mini_display *dpy, struct mini_surface *surf) {
    if (!dpy || !surf) {
        return MINI_ERR_BAD_ARG;
    }
    return dpy->backend->vtbl->swap_buffers(dpy, surf);
}

struct gbm_bo *miniLockFrontBuffer(struct mini_surface *surf) {
    if (!surf || !surf->swapped || surf->front_bo) {
        return NULL;
    }

    surf->front_bo = gbm_surface_lock_front_buffer(surf->gbm_surface);
    if (!surf->front_bo) {
        return NULL;
    }

    surf->swapped = 0;
    return surf->front_bo;
}

void miniReleaseBuffer(struct mini_surface *surf, struct gbm_bo *bo) {
    if (!surf || !bo) {
        return;
    }

    gbm_surface_release_buffer(surf->gbm_surface, bo);
    if (surf->front_bo == bo) {
        surf->front_bo = NULL;
    }
}