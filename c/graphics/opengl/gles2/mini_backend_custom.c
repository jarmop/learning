#include "mini_egl_internal.h"

static int custom_initialize(struct mini_display *dpy) {
    if (!dpy || !dpy->driver_hook) {
        return MINI_ERR_BAD_ARG;
    }

    /*
     * Custom default policy first.
     */
    dpy->default_config.gbm_format   = GBM_FORMAT_XRGB8888;
    dpy->default_config.gles_version = 2;
    dpy->default_config.depth_bits   = 0;
    dpy->default_config.stencil_bits = 0;
    dpy->default_config.has_alpha    = 0;

    int rc = dpy->driver_hook->vtbl->init_driver(dpy);
    if (rc != MINI_OK) {
        return rc;
    }

    /*
     * Explicitly ask the driver hook which native format is required
     * for its drawable creation path.
     */
    if (dpy->driver_hook->vtbl->query_native_format) {
        uint32_t fmt = 0;
        rc = dpy->driver_hook->vtbl->query_native_format(dpy, &fmt);
        if (rc != MINI_OK) {
            return rc;
        }

        dpy->default_config.gbm_format = fmt;
        dpy->default_config.has_alpha =
            (fmt == GBM_FORMAT_ARGB8888 || fmt == GBM_FORMAT_ABGR8888) ? 1 : 0;
    }

    return MINI_OK;
}

static void custom_terminate(struct mini_display *dpy) {
    if (!dpy || !dpy->driver_hook) {
        return;
    }

    dpy->driver_hook->vtbl->shutdown_driver(dpy);
}

static int custom_create_context(struct mini_context *ctx) {
    if (!ctx || !ctx->dpy || !ctx->dpy->driver_hook) {
        return MINI_ERR_BAD_ARG;
    }

    return ctx->dpy->driver_hook->vtbl->create_context(ctx);
}

static void custom_destroy_context(struct mini_context *ctx) {
    if (!ctx || !ctx->dpy || !ctx->dpy->driver_hook) {
        return;
    }

    ctx->dpy->driver_hook->vtbl->destroy_context(ctx);
}

static int custom_create_surface(struct mini_surface *surf) {
    if (!surf || !surf->dpy || !surf->dpy->driver_hook || !surf->dpy->gbm) {
        return MINI_ERR_BAD_ARG;
    }

    surf->front_bo = NULL;
    surf->swapped = 0;

    /*
     * Native surface creation is now fully owned by the custom layer.
     */
    surf->gbm_surface = gbm_surface_create(
        surf->dpy->gbm,
        surf->width,
        surf->height,
        surf->format,
        GBM_BO_USE_SCANOUT | GBM_BO_USE_RENDERING
    );

    if (!surf->gbm_surface) {
        return MINI_ERR_BAD_STATE;
    }

    return surf->dpy->driver_hook->vtbl->create_drawable(surf);
}

static void custom_destroy_surface(struct mini_surface *surf) {
    if (!surf || !surf->dpy || !surf->dpy->driver_hook) {
        return;
    }

    if (surf->front_bo) {
        gbm_surface_release_buffer(surf->gbm_surface, surf->front_bo);
        surf->front_bo = NULL;
    }

    surf->dpy->driver_hook->vtbl->destroy_drawable(surf);

    if (surf->gbm_surface) {
        gbm_surface_destroy(surf->gbm_surface);
        surf->gbm_surface = NULL;
    }
}

static int custom_make_current(
    struct mini_display *dpy,
    struct mini_surface *draw,
    struct mini_surface *read,
    struct mini_context *ctx
) {
    if (!dpy || !dpy->driver_hook) {
        return MINI_ERR_BAD_ARG;
    }

    if (!draw && !read && !ctx) {
        return dpy->driver_hook->vtbl->unbind_current(dpy);
    }

    if (!draw || !read || !ctx) {
        return MINI_ERR_BAD_ARG;
    }

    if (draw->dpy != dpy || read->dpy != dpy || ctx->dpy != dpy) {
        return MINI_ERR_BAD_ARG;
    }

    return dpy->driver_hook->vtbl->bind_current(dpy, draw, read, ctx);
}

static int custom_swap_buffers(struct mini_display *dpy, struct mini_surface *surf) {
    if (!dpy || !surf || !dpy->driver_hook) {
        return MINI_ERR_BAD_ARG;
    }

    if (g_mini_current.dpy != dpy || g_mini_current.draw != surf || !g_mini_current.ctx) {
        return MINI_ERR_BAD_STATE;
    }

    int rc = dpy->driver_hook->vtbl->flush_rendering(dpy, surf);
    if (rc != MINI_OK) {
        return rc;
    }

    rc = dpy->driver_hook->vtbl->publish_surface(dpy, surf);
    if (rc != MINI_OK) {
        return rc;
    }

    if (surf->front_bo) {
        return MINI_ERR_BAD_STATE;
    }

    rc = dpy->driver_hook->vtbl->acquire_front_buffer(surf, &surf->front_bo);
    if (rc != MINI_OK) {
        return rc;
    }

    surf->swapped = 1;
    return MINI_OK;
}

static const struct mini_backend_vtbl custom_vtbl = {
    .initialize = custom_initialize,
    .terminate = custom_terminate,
    .create_context = custom_create_context,
    .destroy_context = custom_destroy_context,
    .create_surface = custom_create_surface,
    .destroy_surface = custom_destroy_surface,
    .make_current = custom_make_current,
    .swap_buffers = custom_swap_buffers,
};

const struct mini_backend mini_backend_custom = {
    .name = "custom",
    .vtbl = &custom_vtbl,
};