#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>

#include <wayland-server-core.h>
#include <wayland-server-protocol.h>

#include "xdg-shell-server-protocol.h"

/* ------------------------------------------------ */
/* Globals */

struct state {
    struct wl_display *display;
    struct wl_event_loop *loop;
    struct shm_pool *pool;
};

static struct state state;

/* ------------------------------------------------ */
/* wl_buffer implementation */

static void buffer_destroy(struct wl_client *client, struct wl_resource *resource) {
    fprintf(stderr, "buffer_destroy\n");
    wl_resource_destroy(resource);
}

static const struct wl_buffer_interface buffer_impl = {
    .destroy = buffer_destroy,
};

/* ------------------------------------------------ */
/* wl_shm_pool implementation */

struct shm_pool {
    void *data;
    size_t size;
};

static void shm_pool_destroy(struct wl_client *client, struct wl_resource *resource) {
    fprintf(stderr, "shm_pool_destroy\n");
    struct shm_pool *pool = wl_resource_get_user_data(resource);
    munmap(pool->data, pool->size);
    free(pool);
    wl_resource_destroy(resource);
}

static void shm_pool_create_buffer(
    struct wl_client *client,
    struct wl_resource *resource,
    uint32_t id,
    int32_t offset,
    int32_t width,
    int32_t height,
    int32_t stride,
    uint32_t format
) {
    fprintf(stderr, "shm_pool_create_buffer\n");
    struct wl_resource *buffer = wl_resource_create(client, &wl_buffer_interface, 1, id);
    wl_resource_set_implementation(buffer, &buffer_impl, NULL, NULL);
}

static void shm_pool_resize(struct wl_client *client, struct wl_resource *resource, int32_t size) {
    fprintf(stderr, "shm_pool_resize\n");
}

static const struct wl_shm_pool_interface shm_pool_impl = {
    .destroy = shm_pool_destroy,
    .create_buffer = shm_pool_create_buffer,
    .resize = shm_pool_resize,
};

/* ------------------------------------------------ */
/* wl_shm implementation */

static void shm_create_pool(
    struct wl_client *client,
    struct wl_resource *resource,
    uint32_t id,
    int fd,
    int32_t size
) {
    fprintf(stderr, "shm_create_pool\n");
    struct shm_pool *pool = calloc(1, sizeof *pool);
    pool->size = size;
    pool->data = mmap(NULL, size, PROT_READ, MAP_SHARED, fd, 0);
    state.pool = pool;

    struct wl_resource *res =
        wl_resource_create(client, &wl_shm_pool_interface, 1, id);

    wl_resource_set_implementation(res, &shm_pool_impl, pool, NULL);
}

static const struct wl_shm_interface shm_impl = {
    .create_pool = shm_create_pool,
};

/* ------------------------------------------------ */
/* wl_surface implementation */

static void surface_destroy(struct wl_client *client, struct wl_resource *resource) {
    fprintf(stderr, "surface_destroy\n");
    wl_resource_destroy(resource);
}

static void surface_attach(
    struct wl_client *client,
    struct wl_resource *resource,
    struct wl_resource *buffer,
    int32_t x,
    int32_t y
) {
    fprintf(stderr, "surface_attach\n");
    (void)client;
    (void)resource;
    // (void)buffer;
    (void)x;
    (void)y;
}

static void surface_damage(
    struct wl_client *client,
    struct wl_resource *resource,
    int32_t x,
    int32_t y,
    int32_t width,
    int32_t height
) {
    fprintf(stderr, "surface_damage\n");
}

static void surface_commit(struct wl_client *client, struct wl_resource *resource) {
    fprintf(stderr, "surface_commit\n");
    struct shm_pool *pool = state.pool;
    uint32_t *pool_data = pool->data;
    fprintf(stderr, "pool data: %x\n", pool_data[0]);
    (void)client;
    (void)resource;
}

static const struct wl_surface_interface surface_impl = {
    .destroy = surface_destroy, // opcode 0
    .attach = surface_attach, // opcode 1
    .damage = surface_damage, // opcode 2
    .commit = surface_commit,
};

/* ------------------------------------------------ */
/* wl_compositor implementation */

static void compositor_create_surface(struct wl_client *client, struct wl_resource *resource, uint32_t id) {
    fprintf(stderr, "compositor_create_surface\n");
    struct wl_resource *surface = wl_resource_create(client, &wl_surface_interface, 4, id);
    wl_resource_set_implementation(surface, &surface_impl, NULL, NULL);
}

static const struct wl_compositor_interface compositor_impl = {
    .create_surface = compositor_create_surface,
    .create_region = NULL,
};

/* ------------------------------------------------ */
/* xdg_surface and xdg_toplevel implementations */

static void xdg_toplevel_destroy(struct wl_client *client, struct wl_resource *resource) {
    fprintf(stderr, "xdg_toplevel_destroy\n");
    wl_resource_destroy(resource);
}

static const struct xdg_toplevel_interface xdg_toplevel_impl = {
    .destroy = xdg_toplevel_destroy,
};

static void xdg_surface_get_toplevel(struct wl_client *client, struct wl_resource *resource, uint32_t id) {
    fprintf(stderr, "xdg_surface_get_toplevel\n");
    struct wl_resource *toplevel = wl_resource_create(client, &xdg_toplevel_interface, 1, id);
    wl_resource_set_implementation(toplevel, &xdg_toplevel_impl, NULL, NULL);
}

static void xdg_surface_destroy(struct wl_client *client, struct wl_resource *resource) {
    fprintf(stderr, "xdg_surface_destroy\n");
    wl_resource_destroy(resource);
}

static void xdg_surface_ack_configure(struct wl_client *client, struct wl_resource *resource, uint32_t serial) {
    fprintf(stderr, "xdg_surface_ack_configure\n");
    (void)client;
    (void)resource;
    (void)serial;
}

static const struct xdg_surface_interface xdg_surface_impl = {
    .destroy = xdg_surface_destroy, // opcode 0
    .get_toplevel = xdg_surface_get_toplevel, // opcode 1
    .ack_configure = xdg_surface_ack_configure, // opcode 4
};

/* ------------------------------------------------ */
/* xdg_wm_base implementation */

static void xdg_wm_base_destroy(struct wl_client *client, struct wl_resource *resource) {
    fprintf(stderr, "xdg_wm_base_destroy\n");
    wl_resource_destroy(resource);
}

static void xdg_wm_base_get_xdg_surface(
    struct wl_client *client,
    struct wl_resource *resource,
    uint32_t id,
    struct wl_resource *surface
) {
    struct wl_resource *xdg_surface = wl_resource_create(client, &xdg_surface_interface, 1, id);

    wl_resource_set_implementation(xdg_surface, &xdg_surface_impl, surface, NULL);

    uint32_t serial = wl_display_next_serial(state.display);
    xdg_surface_send_configure(xdg_surface, serial);
}

static void xdg_wm_base_pong(struct wl_client *client, struct wl_resource *resource, uint32_t serial) {
    fprintf(stderr, "xdg_wm_base_pong\n");
    (void)client;
    (void)resource;
    (void)serial;
}

static const struct xdg_wm_base_interface xdg_wm_base_impl = {
    .destroy = xdg_wm_base_destroy,
    .create_positioner = NULL,
    .get_xdg_surface = xdg_wm_base_get_xdg_surface,
    .pong = xdg_wm_base_pong,
};

/* ------------------------------------------------ */
/* Registry binders */

static void bind_compositor(struct wl_client *client, void *data, uint32_t version, uint32_t id) {
    fprintf(stderr, "bind_compositor\n");
    struct wl_resource *res = wl_resource_create(client, &wl_compositor_interface, 4, id);
    wl_resource_set_implementation(res, &compositor_impl, NULL, NULL);
}

static void bind_shm(struct wl_client *client, void *data, uint32_t version, uint32_t id) {
    fprintf(stderr, "bind_shm\n");
    struct wl_resource *res = wl_resource_create(client, &wl_shm_interface, 1, id);
    wl_resource_set_implementation(res, &shm_impl, NULL, NULL);
    wl_shm_send_format(res, WL_SHM_FORMAT_XRGB8888);
}

static void bind_xdg_wm_base(struct wl_client *client, void *data, uint32_t version, uint32_t id) {
    fprintf(stderr, "bind_xdg_wm_base\n");
    struct wl_resource *res = wl_resource_create(client, &xdg_wm_base_interface, 1, id);
    wl_resource_set_implementation(res, &xdg_wm_base_impl, NULL, NULL);
}

/* ------------------------------------------------ */
/* Main */

int main(void) {
    state.display = wl_display_create();
    state.loop = wl_display_get_event_loop(state.display);

    wl_global_create(state.display, &wl_compositor_interface, 4, NULL, bind_compositor);
    wl_global_create(state.display, &wl_shm_interface, 1, NULL, bind_shm);
    wl_global_create(state.display, &xdg_wm_base_interface, 1, NULL, bind_xdg_wm_base);

    const char *socket = wl_display_add_socket_auto(state.display);
    if (!socket) {
        fprintf(stderr, "Failed to create socket\n");
        return 1;
    }

    fprintf(stderr, "Wayland compositor running on %s\n", socket);
    wl_display_run(state.display);

    wl_display_destroy(state.display);
    return 0;
}
