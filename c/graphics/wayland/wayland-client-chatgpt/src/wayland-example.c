#define _GNU_SOURCE

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/mman.h>

#include "xdg-shell-client-protocol.h"
#include "wayland-example.h"

static struct wl_compositor *compositor; // For wl_surface
static struct wl_shm *shm; // For allocating shared memory buffers
static struct xdg_wm_base *wm_base; // For creating xdg-shell surfaces

static void xdg_wm_base_ping(
    void *data, 
    struct xdg_wm_base *xdg_wm_base, 
    uint32_t serial
) {
    xdg_wm_base_pong(xdg_wm_base, serial);
}

static const struct xdg_wm_base_listener xdg_wm_base_listener = {
    .ping = xdg_wm_base_ping,
};

static void __registry_handler(
    void *data,
    struct wl_registry *registry,
    uint32_t id, const char *interface,
    uint32_t version
) {
    if (strcmp(interface, wl_compositor_interface.name) == 0) {
        compositor = wl_registry_bind(registry, id, &wl_compositor_interface, 4);
    } else if (strcmp(interface, wl_shm_interface.name) == 0) {
        shm = wl_registry_bind(registry, id, &wl_shm_interface, 1);
    } else if (strcmp(interface, xdg_wm_base_interface.name) == 0) {
        wm_base = wl_registry_bind(registry, id, &xdg_wm_base_interface, 1);
        xdg_wm_base_add_listener(wm_base, &xdg_wm_base_listener, data);
    }
}

static void __registry_remove(void *data, struct wl_registry *registry, uint32_t id) {}

static const struct wl_registry_listener registry_listener = {
    .global = __registry_handler,
    .global_remove = __registry_remove
};

static struct wl_buffer *__create_buffer(int width, int height) {
    int stride = width * 4;
    int size = stride * height;

    int fd = memfd_create("shm-buffer", 0);
    ftruncate(fd, size);

    void *data = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    // A pool of pixel memory
    struct wl_shm_pool *pool = wl_shm_create_pool(shm, fd, size);
    // A single image inside the pool
    struct wl_buffer *buffer = wl_shm_pool_create_buffer(
        pool, 0, width, height, stride, WL_SHM_FORMAT_XRGB8888
    );
    // The buffer is still shared by the compositor when the shared pool is destroyed
    wl_shm_pool_destroy(pool);

    // Create the image
    uint32_t *p = data;
    for (int i = 0; i < width * height; i++) {
        p[i] = 0xff404040;  // gray
    }

    return buffer;
}

int main() {
    struct wl_display *display = my_wl_display_connect(NULL);
    if (!display) {
        fprintf(stderr, "No Wayland display\n");
        return 1;
    }

    /**
     * Initialize the global "compositor", "shm" and "wm_base" variables.
     * Registry_listener uses wl_compositor and wl_registry protocols
     */ 
    struct wl_registry *registry = my_wl_display_get_registry(display);
    wl_registry_add_listener(registry, &registry_listener, NULL);
    wl_display_roundtrip(display);
    if (!compositor || !shm || !wm_base) {
        fprintf(stderr, "Wayland globals missing\n");
        return 1;
    }

    // Create surfaces
    struct wl_surface *surface = wl_compositor_create_surface(compositor);
    struct xdg_surface *xdg_surface = xdg_wm_base_get_xdg_surface(wm_base, surface);
    struct xdg_toplevel *toplevel = xdg_surface_get_toplevel(xdg_surface);
    // xdg_toplevel_set_title(toplevel, "Wayland Hello");
    // wl_surface_commit(surface);

    struct wl_buffer *buffer = __create_buffer(400, 300);

    wl_surface_attach(surface, buffer, 0, 0);
    wl_surface_commit(surface);

    while (1) {
        wl_display_dispatch(display);
    }

    return 0;
}
