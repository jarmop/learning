#define _GNU_SOURCE

#include <wayland-client.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>

/* xdg-shell */
#include "xdg-shell-client-protocol.h"

static struct wl_compositor *compositor;
static struct wl_shm *shm;
static struct xdg_wm_base *wm_base;

static void __registry_handler(
    void *data, struct wl_registry *registry,
    uint32_t id, const char *interface, uint32_t version)
{
    if (strcmp(interface, wl_compositor_interface.name) == 0)
        compositor = wl_registry_bind(registry, id,
                                       &wl_compositor_interface, 4);
    else if (strcmp(interface, wl_shm_interface.name) == 0)
        shm = wl_registry_bind(registry, id,
                               &wl_shm_interface, 1);
    else if (strcmp(interface, xdg_wm_base_interface.name) == 0)
        wm_base = wl_registry_bind(registry, id,
                                   &xdg_wm_base_interface, 1);
}

static void __registry_remove(void *data,
                            struct wl_registry *registry,
                            uint32_t id) {}

static const struct wl_registry_listener registry_listener = {
    .global = __registry_handler,
    .global_remove = __registry_remove
};

/* ---------- Shared memory buffer ---------- */

static int __create_shm_file(size_t size) {
    int fd = memfd_create("shm-buffer", 0);
    ftruncate(fd, size);
    return fd;
}

static struct wl_buffer *__create_buffer(int width, int height) {
    int stride = width * 4;
    int size = stride * height;

    int fd = __create_shm_file(size);
    void *data = mmap(NULL, size,
                      PROT_READ | PROT_WRITE,
                      MAP_SHARED, fd, 0);

    struct wl_shm_pool *pool =
        wl_shm_create_pool(shm, fd, size);

    struct wl_buffer *buffer =
        wl_shm_pool_create_buffer(
            pool, 0, width, height, stride,
            WL_SHM_FORMAT_XRGB8888);

    wl_shm_pool_destroy(pool);

    uint32_t *p = data;
    for (int i = 0; i < width * height; i++)
        p[i] = 0xff404040;  // gray

    return buffer;
}

int main() {
    struct wl_display *display = wl_display_connect(NULL);
    if (!display) {
        fprintf(stderr, "No Wayland display\n");
        return 1;
    }

    struct wl_registry *registry =
        wl_display_get_registry(display);

    wl_registry_add_listener(registry,
                             &registry_listener, NULL);

    wl_display_roundtrip(display);

    if (!compositor || !shm || !wm_base) {
        fprintf(stderr, "Wayland globals missing\n");
        return 1;
    }

    struct wl_surface *surface =
        wl_compositor_create_surface(compositor);

    struct xdg_surface *xdg_surface =
        xdg_wm_base_get_xdg_surface(wm_base, surface);

    struct xdg_toplevel *toplevel =
        xdg_surface_get_toplevel(xdg_surface);

    xdg_toplevel_set_title(toplevel, "Wayland Hello");

    wl_surface_commit(surface);

    struct wl_buffer *buffer = __create_buffer(400, 300);

    wl_surface_attach(surface, buffer, 0, 0);
    wl_surface_commit(surface);

    for (int i = 0; i < 300; i++)
        wl_display_dispatch(display);

    return 0;
}
