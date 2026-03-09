#include "mini_egl_internal.h"

static int stub_initialize(struct mini_display *dpy) {
    (void)dpy;
    return MINI_OK;
}

static void stub_terminate(struct mini_display *dpy) {
    (void)dpy;
}

static int stub_create_context(struct mini_context *ctx) {
    ctx->backend_data = NULL;
    return MINI_OK;
}

static void stub_destroy_context(struct mini_context *ctx) {
    (void)ctx;
}

static int stub_create_surface(struct mini_surface *surf) {
    surf->backend_data = NULL;
    surf->front_bo = NULL;
    surf->swapped = 0;
    return MINI_OK;
}

static void stub_destroy_surface(struct mini_surface *surf) {
    (void)surf;
}

static int stub_make_current(
    struct mini_display *dpy,
    struct mini_surface *draw,
    struct mini_surface *read,
    struct mini_context *ctx
) {
    if (!draw && !read && !ctx) {
        return MINI_OK;
    }

    if (!ctx || !draw || !read) {
        return MINI_ERR_BAD_ARG;
    }

    if (ctx->dpy != dpy || draw->dpy != dpy || read->dpy != dpy) {
        return MINI_ERR_BAD_ARG;
    }

    return MINI_OK;
}

static int stub_swap_buffers(struct mini_display *dpy, struct mini_surface *surf) {
    if (g_mini_current.dpy != dpy || g_mini_current.draw != surf || !g_mini_current.ctx) {
        return MINI_ERR_BAD_STATE;
    }

    surf->swapped = 1;
    return MINI_OK;
}

static const struct mini_backend_vtbl stub_vtbl = {
    .initialize = stub_initialize,
    .terminate = stub_terminate,
    .create_context = stub_create_context,
    .destroy_context = stub_destroy_context,
    .create_surface = stub_create_surface,
    .destroy_surface = stub_destroy_surface,
    .make_current = stub_make_current,
    .swap_buffers = stub_swap_buffers,
};

const struct mini_backend mini_backend_stub = {
    .name = "stub",
    .vtbl = &stub_vtbl,
};