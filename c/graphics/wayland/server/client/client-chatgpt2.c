// wayland-minimal.c
// Minimal Wayland client using wl_shm.
// Creates a 200x100 window and draws a colored rectangle.

// #include <wayland-client.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <string.h>

#include "xdg-shell-client-protocol.h" // includes also wayland-client.h

static struct wl_compositor *compositor = NULL;
static struct wl_shm *shm = NULL;
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

static void registry_global(void *data,
                            struct wl_registry *registry,
                            uint32_t name,
                            const char *interface,
                            uint32_t version)
{
    if (strcmp(interface, "wl_compositor") == 0) {
        compositor = wl_registry_bind(
            registry, name, &wl_compositor_interface, 4);
    }
    else if (strcmp(interface, "wl_shm") == 0) {
        shm = wl_registry_bind(
            registry, name, &wl_shm_interface, 1);
    } else if (strcmp(interface, xdg_wm_base_interface.name) == 0) {
        wm_base = wl_registry_bind(registry, name, &xdg_wm_base_interface, 1);
        xdg_wm_base_add_listener(wm_base, &xdg_wm_base_listener, data);
    }
}

static void registry_remove(void *data,
                            struct wl_registry *registry,
                            uint32_t name)
{
    // not needed
}

static struct wl_registry_listener registry_listener = {
    .global = registry_global,
    .global_remove = registry_remove
};

// ------------ Helper to create shared memory buffer -------------

static int create_shm_file(size_t size)
{
    char template[] = "/tmp/wayland-shm-XXXXXX";
    int fd = mkstemp(template);

    if (fd < 0) {
        perror("mkstemp");
        exit(1);
    }

    unlink(template);
    if (ftruncate(fd, size) < 0) {
        perror("ftruncate");
        close(fd);
        exit(1);
    }

    return fd;
}

int main() {
    struct wl_display *display = wl_display_connect(NULL);
    if (!display) {
        fprintf(stderr, "Failed to connect to Wayland display.\n");
        return 1;
    }

    struct wl_registry *registry = wl_display_get_registry(display);
    wl_registry_add_listener(registry, &registry_listener, NULL);

    wl_display_roundtrip(display); // get globals

    if (!compositor || !shm || !wm_base) {
        fprintf(stderr, "Compositor or SHM not available.\n");
        return 1;
    }

    // Create surfaces
    struct wl_surface *surface = wl_compositor_create_surface(compositor);
    struct xdg_surface *xdg_surface = xdg_wm_base_get_xdg_surface(wm_base, surface);
    struct xdg_toplevel *toplevel = xdg_surface_get_toplevel(xdg_surface);

    // Create shm pool + buffer
    int width = 200;
    int height = 100;
    int stride = width * 4;
    size_t size = stride * height;

    int fd = create_shm_file(size);
    uint32_t *data = mmap(NULL, size,
                          PROT_READ | PROT_WRITE,
                          MAP_SHARED, fd, 0);

    struct wl_shm_pool *pool = wl_shm_create_pool(shm, fd, size);
    struct wl_buffer *buffer = wl_shm_pool_create_buffer(
        pool, 0,
        width, height,
        stride,
        WL_SHM_FORMAT_XRGB8888);

    wl_shm_pool_destroy(pool);
    close(fd);

    // Draw a simple rectangle (blue background)
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            data[y * width + x] = 0xff0000ff; // XRGB: Blue
        }
    }

    // Attach buffer to surface
    wl_surface_attach(surface, buffer, 0, 0);
    wl_surface_commit(surface);

    // Main event loop
    while (wl_display_dispatch(display) != -1) {
        // idle
    }

    // munmap(data, size);
    // wl_display_disconnect(display);
    return 0;
}