#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <time.h>
#include <unistd.h>
#include <wayland-client.h>
#include "xdg-shell-client-protocol.h"

/* Shared memory support code */
static void randname(char *buf) {
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    long r = ts.tv_nsec;
    for (int i = 0; i < 6; ++i) {
        buf[i] = 'A'+(r&15)+(r&16)*2;
        r >>= 5;
    }
}

static int create_shm_file(void) {
    int retries = 100;
    do {
        char name[] = "/wl_shm-XXXXXX";
        // Replace the X characters with a random character
        randname(name + sizeof(name) - 7);
        --retries;
        int fd = shm_open(name, O_RDWR | O_CREAT | O_EXCL, 0600);
        if (fd >= 0) {
            shm_unlink(name);
            return fd;
        }
    } while (retries > 0 && errno == EEXIST);
    return -1;
}

static int allocate_shm_file(size_t size) {
    int fd = create_shm_file();
    if (fd < 0)
        return -1;
    int ret;
    do {
        ret = ftruncate(fd, size);
    } while (ret < 0 && errno == EINTR);
    if (ret < 0) {
        close(fd);
        return -1;
    }
    return fd;
}

struct client_state {
    /* Globals */
    struct wl_compositor *compositor;
    struct wl_shm *shm;
    struct xdg_wm_base *xdg_wm_base;
    /* Objects */
};

static struct wl_buffer* draw_frame(
    struct client_state *state, 
    struct wl_surface *surface
) {
    const int width = 640, height = 480;
    /**
     * Stride specifies the number of bytes from the beginning of one row to 
     * the beginning of the next. XRGB8888 format uses 4 bytes per pixel.
     */
    const int stride = width * 4;
    // Two buffers for double buffering
    const int shm_pool_size = height * stride * 2;

    int fd = allocate_shm_file(shm_pool_size);
    uint8_t *pool_data = mmap(
        NULL, shm_pool_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0
    );

    struct wl_shm_pool *pool = wl_shm_create_pool(state->shm, fd, shm_pool_size);

    int offset = 0;
    struct wl_buffer *buffer = wl_shm_pool_create_buffer(
        pool, offset, width, height, stride, WL_SHM_FORMAT_XRGB8888
    );

    // uint32_t *pixels = (uint32_t *)&pool_data[offset];
    // memset(pixels, 0, width * height * 4);

    uint32_t *pixels = (uint32_t *)&pool_data[offset];
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if ((x + y / 8 * 8) % 16 < 8) {
                pixels[y * width + x] = 0xFF666666;
            } else {
                pixels[y * width + x] = 0xFFEEEEEE;
            }
        }
    }

    wl_surface_attach(surface, buffer, 0, 0);
    wl_surface_damage(surface, 0, 0, UINT32_MAX, UINT32_MAX);
    wl_surface_commit(surface);
}

static void registry_global(
    void *data, 
    struct wl_registry *wl_registry,
	uint32_t name, 
    const char *interface, 
    uint32_t version
) {
	// printf("interface: '%s', version: %d, name: %d\n", interface, version, name);

    struct client_state *state = data;
    if (strcmp(interface, wl_compositor_interface.name) == 0) {
        state->compositor = wl_registry_bind(wl_registry, name, &wl_compositor_interface, 4);
    } else if (strcmp(interface, wl_shm_interface.name) == 0) {
        state->shm = wl_registry_bind(wl_registry, name, &wl_shm_interface, 1);
    } else if (strcmp(interface, xdg_wm_base_interface.name) == 0) {
        state->xdg_wm_base = wl_registry_bind(wl_registry, name, &xdg_wm_base_interface, 1);
    }
}

static void registry_global_remove(
    void *data, 
    struct wl_registry *registry,
	uint32_t name
) {
	// This space deliberately left blank
}

static const struct wl_registry_listener registry_listener = {
	.global = registry_global,
	.global_remove = registry_global_remove,
};

int main(int argc, char *argv[]) {
    struct wl_display *display = wl_display_connect(NULL);
    if (!display) {
        fprintf(stderr, "Failed to connect to Wayland display.\n");
        return 1;
    }

    struct wl_registry *registry = wl_display_get_registry(display);

    struct client_state state = {0};

	wl_registry_add_listener(registry, &registry_listener, &state);
	wl_display_roundtrip(display);

    struct wl_surface *surface = wl_compositor_create_surface(state.compositor);
    struct xdg_surface *xdg_surface = xdg_wm_base_get_xdg_surface(state.xdg_wm_base, surface);
    struct xdg_toplevel *xdg_toplevel = xdg_surface_get_toplevel(xdg_surface);

    draw_frame(&state, surface);

    wl_surface_commit(surface);

    while(wl_display_dispatch(display)) {

    }


    // wl_display_disconnect(display);

    return 0;
}