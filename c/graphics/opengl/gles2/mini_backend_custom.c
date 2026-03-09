#include "mini_egl_internal.h"

static int custom_initialize(struct mini_display *dpy) {
    if (!dpy || !dpy->driver_hook) {
        return MINI_ERR_BAD_ARG;
    }

    /*
     * Custom policy:
     * one hardcoded config for now.
     * Later you can expand this without touching the driver hook.
     */
    dpy->default_config.gbm_format   = GBM_FORMAT_XRGB8888;
    dpy->default_config.gles_version = 2;
    dpy->default_config.depth_bits   = 0;
    dpy->default_config.stencil_bits = 0;
    dpy->default_config.has_alpha    = 0;

    return dpy->driver_hook->vtbl->init_driver(dpy);
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
    if (!surf || !surf->dpy || !surf->dpy->driver_hook) {
        return MINI_ERR_BAD_ARG;
    }

    surf->front_bo = NULL;
    surf->swapped = 0;

    /*
     * Custom policy check:
     * require native format match with chosen config.
     * For now the caller is responsible for creating the gbm_surface
     * with cfg->gbm_format.
     */

    return surf->dpy->driver_hook->vtbl->create_drawable(surf);
}

static void custom_destroy_surface(struct mini_surface *surf) {
    if (!surf || !surf->dpy || !surf->dpy->driver_hook) {
        return;
    }

    surf->dpy->driver_hook->vtbl->destroy_drawable(surf);
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

    int rc = dpy->driver_hook->vtbl->flush_and_publish(dpy, surf);
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