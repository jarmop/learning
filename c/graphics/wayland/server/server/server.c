#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <linux/input.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
#include <sys/time.h>

#include <wayland-server-core.h>
#include <wayland-server-protocol.h>

#include "xdg-shell-server-protocol.h"

#include "display.h"
#include "renderer.h"
#include "types.h"

/**
 * Need to store the surface data for the mouse cursor as well. So there we have
 * our other client: the desktop environment. Or rather, the desktop environment 
 * provides multiple clients like the mouse cursor, and the menu bar, and such. 
 * Would it make sense to redraw each surface whenever one of the changes, or 
 * should we try to erase the old version of the surface and then just draw the 
 * new version and keep the rest of the surfaces as is? Probably best, to first 
 * go with the immediate mode and change to retained mode only when needed.
 */
// struct surface_data {
//     void *pixels;
//     size_t size;
//     uint32_t stride;
//     int x;
//     int y;
// };

struct wl_shm_buffer *surface_buffer;
int surface_x = 0;
int surface_y = 0;

struct wl_display *wl_display;
struct wl_event_loop *loop;
// struct surface_data *surface_data;

struct pxgrid drm = {0};
struct pxgrid background = {0};
struct pxgrid mouse = {0};
struct pxgrid surface = {0};

/* ------------------------------------------------ */
/* wl_buffer implementation */

static void buffer_destroy(struct wl_client *client, struct wl_resource *resource) {
    // fprintf(stderr, "buffer_destroy\n");
    wl_resource_destroy(resource);
}

static const struct wl_buffer_interface buffer_impl = {
    .destroy = buffer_destroy,
};

/* ------------------------------------------------ */
/* wl_shm_pool implementation */

// static void shm_pool_destroy(struct wl_client *client, struct wl_resource *resource) {
//     // fprintf(stderr, "shm_pool_destroy\n");
//     struct surface_data *pool = wl_resource_get_user_data(resource);
//     munmap(pool->pixels, pool->size);
//     free(pool);
//     wl_resource_destroy(resource);
// }

// static void shm_pool_create_buffer(
//     struct wl_client *client,
//     struct wl_resource *resource,
//     uint32_t id,
//     int32_t offset,
//     int32_t width,
//     int32_t height,
//     int32_t stride,
//     uint32_t format
// ) {
//     fprintf(stderr, "shm_pool_create_buffer: object id %d\n", resource->object.id);
//     struct wl_resource *buffer = wl_resource_create(client, &wl_buffer_interface, 1, id);
//     wl_resource_set_implementation(buffer, &buffer_impl, NULL, NULL);

//     // Store the stride to the surface_data like a boss. Let's see when this causes problems
//     surface_data->stride = stride;
// }

// static void shm_pool_resize(struct wl_client *client, struct wl_resource *resource, int32_t size) {
//     // fprintf(stderr, "shm_pool_resize\n");
// }

// static const struct wl_shm_pool_interface shm_pool_impl = {
//     .destroy = shm_pool_destroy,
//     .create_buffer = shm_pool_create_buffer,
//     .resize = shm_pool_resize,
// };

// /* ------------------------------------------------ */
// /* wl_shm implementation */

// static void shm_create_pool(
//     struct wl_client *client,
//     struct wl_resource *resource,
//     uint32_t id,
//     int fd,
//     int32_t size
// ) {
//     fprintf(stderr, "shm_create_pool: id: %d, fd: %d, size: %d\n", id, fd, size);
//     surface_data = calloc(1, sizeof *surface_data);
//     surface_data->size = size;
//     surface_data->pixels = mmap(NULL, size, PROT_READ, MAP_SHARED, fd, 0);

//     struct wl_resource *res =
//         wl_resource_create(client, &wl_shm_pool_interface, 1, id);

//     wl_resource_set_implementation(res, &shm_pool_impl, surface_data, NULL);
// }

// static const struct wl_shm_interface shm_impl = {
//     .create_pool = shm_create_pool,
// };

/* ------------------------------------------------ */
/* wl_surface implementation */

static void surface_destroy(struct wl_client *client, struct wl_resource *resource) {
    // fprintf(stderr, "surface_destroy\n");
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
    surface_buffer = wl_shm_buffer_get(buffer);
    uint32_t *pixels = wl_shm_buffer_get_data(surface_buffer);
    uint32_t stride = wl_shm_buffer_get_stride(surface_buffer);
    uint32_t width = wl_shm_buffer_get_width(surface_buffer);
    uint32_t height = wl_shm_buffer_get_height(surface_buffer);
    surface_x = x;
    surface_y = y;

    surface.pixels = pixels;
    surface.width = width;
    surface.height = height;
    surface.x = x;
    surface.y = y;

    // fprintf(stderr, "surface_attach – resource->object.id: %d\n", resource->object.id);
    // surface_data->x = x;
    // surface_data->y = y;
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
    fprintf(stderr, "surface_commit, resource->object.id: %d\n", resource->object.id);

    // uint32_t *pixels = wl_shm_buffer_get_data(surface_buffer);
    // uint32_t stride = wl_shm_buffer_get_stride(surface_buffer);
    // uint32_t width = wl_shm_buffer_get_width(surface_buffer);
    // uint32_t height = wl_shm_buffer_get_height(surface_buffer);
    // fprintf(stderr, "data: %x, stride: %d, width: %d, height: %d\n", pixels[0], stride, width, height);

    // struct pxgrid surface = {0};
    // surface.pixels = pixels;
    // surface.width = width;
    // surface.height = height;

    rd_blend(&drm, &background);
    rd_blend(&drm, &surface);
    rd_blend(&drm, &mouse);
    drm.pixels = drm_refresh();
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
    // fprintf(stderr, "compositor_create_surface\n");
    struct wl_resource *surface = wl_resource_create(client, &wl_surface_interface, 4, id);
    wl_resource_set_implementation(surface, &surface_impl, NULL, NULL);
}

static const struct wl_compositor_interface compositor_impl = {
    .create_surface = compositor_create_surface,
    .create_region = NULL,
};

/* ------------------------ XDG ------------------------ */

static void xdg_toplevel_destroy(struct wl_client *client, struct wl_resource *resource) {
    // fprintf(stderr, "xdg_toplevel_destroy\n");
    wl_resource_destroy(resource);
}

static const struct xdg_toplevel_interface xdg_toplevel_impl = {
    .destroy = xdg_toplevel_destroy,
};

static void xdg_surface_get_toplevel(struct wl_client *client, struct wl_resource *resource, uint32_t id) {
    // fprintf(stderr, "xdg_surface_get_toplevel\n");
    struct wl_resource *toplevel = wl_resource_create(client, &xdg_toplevel_interface, 1, id);
    wl_resource_set_implementation(toplevel, &xdg_toplevel_impl, NULL, NULL);
}

static void xdg_surface_destroy(struct wl_client *client, struct wl_resource *resource) {
    // fprintf(stderr, "xdg_surface_destroy\n");
    wl_resource_destroy(resource);
}

static void xdg_surface_ack_configure(struct wl_client *client, struct wl_resource *resource, uint32_t serial) {
    // fprintf(stderr, "xdg_surface_ack_configure\n");
}

static const struct xdg_surface_interface xdg_surface_impl = {
    .destroy = xdg_surface_destroy, // opcode 0
    .get_toplevel = xdg_surface_get_toplevel, // opcode 1
    .ack_configure = xdg_surface_ack_configure, // opcode 4
};

static void xdg_wm_base_destroy(struct wl_client *client, struct wl_resource *resource) {
    // fprintf(stderr, "xdg_wm_base_destroy\n");
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

    uint32_t serial = wl_display_next_serial(wl_display);
    xdg_surface_send_configure(xdg_surface, serial);
}

static void xdg_wm_base_pong(struct wl_client *client, struct wl_resource *resource, uint32_t serial) {
    // fprintf(stderr, "xdg_wm_base_pong\n");
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
    // fprintf(stderr, "bind_compositor\n");
    struct wl_resource *res = wl_resource_create(client, &wl_compositor_interface, 4, id);
    wl_resource_set_implementation(res, &compositor_impl, NULL, NULL);
}

// static void bind_shm(struct wl_client *client, void *data, uint32_t version, uint32_t id) {
//     // fprintf(stderr, "bind_shm\n");
//     struct wl_resource *res = wl_resource_create(client, &wl_shm_interface, 1, id);
//     wl_resource_set_implementation(res, &shm_impl, NULL, NULL);
//     wl_shm_send_format(res, WL_SHM_FORMAT_XRGB8888);
// }

static void bind_xdg_wm_base(struct wl_client *client, void *data, uint32_t version, uint32_t id) {
    // fprintf(stderr, "bind_xdg_wm_base\n");
    struct wl_resource *res = wl_resource_create(client, &xdg_wm_base_interface, 1, id);
    wl_resource_set_implementation(res, &xdg_wm_base_impl, NULL, NULL);
}

// struct timespec t1;
// uint32_t tms1 = 0;
// uint32_t tms2 = 0;

struct input_event ev;
static int max_abs_x = 65482; int max_abs_y = 65452;
// static int mouse_x = -1; int mouse_y = -1;

/**
 * event_mask   Bit mask telling which event occurred
 *              Possible events: WL_EVENT_READABLE, WL_EVENT_WRITABLE, WL_EVENT_HANGUP, WL_EVENT_ERROR
 */
static int on_mouse_fd(int fd, uint32_t event_mask, void *_)
{
    int needs_redraw = 0;
    if (!(event_mask & WL_EVENT_READABLE)) return 0;


    for (;;) {
        ssize_t n = read(fd, &ev, sizeof ev);

        // clock_gettime(CLOCK_MONOTONIC, &t1);
        // uint32_t tms2 = t1.tv_sec * 1000 + t1.tv_nsec / 1000000;
        // fprintf(stderr, "time diff: %u\n", tms2 - tms1);
        // tms1 = tms2;

        if (n == (ssize_t)sizeof ev) {
            if (ev.type == EV_ABS) {
                if (ev.code == REL_X) {
                    mouse.x = ev.value * drm.width / max_abs_x;
                } else if (ev.code == REL_Y) {
                    mouse.y = ev.value * drm.height / max_abs_y;
                }

                if (mouse.x > 0 && mouse.x < drm.width - 10 && mouse.y > 0 && mouse.y < drm.height - 10) {
                    rd_blend(&drm, &background);
                    rd_blend(&drm, &surface);
                    rd_blend(&drm, &mouse);
                    drm.pixels = drm_refresh();                    
                }
            }
            if (ev.type == EV_KEY && ev.code == BTN_LEFT) {
                // if (ev.value == 1) printf("Left button pressed\n");
                if (ev.value == 0) {
                    // printf("Left button released\n");
                    drm_close();
                    wl_event_loop_destroy(loop);
                }
            }
            
            continue;
        }
        if (n < 0 && (errno == EAGAIN || errno == EWOULDBLOCK)) break;
        if (n == 0) break; // EOF
        if (n < 0) perror("read(mouse)");
        break;
    }

    return 0;
}

int main(void) {
    wl_display = wl_display_create();
    loop = wl_display_get_event_loop(wl_display);

    wl_global_create(wl_display, &wl_compositor_interface, 4, NULL, bind_compositor);
    
    // wl_global_create(display, &wl_shm_interface, 1, NULL, bind_shm);
    wl_display_init_shm(wl_display);
    wl_display_add_shm_format(wl_display, WL_SHM_FORMAT_XRGB8888);

    wl_global_create(wl_display, &xdg_wm_base_interface, 1, NULL, bind_xdg_wm_base);

    const char *socket = wl_display_add_socket_auto(wl_display);
    if (!socket) {
        fprintf(stderr, "Failed to create socket\n");
        return 1;
    }

    drm_init(&drm);

    background.pixels = malloc(drm.width * drm.height * 4);
    background.width = drm.width;
    background.height = drm.height;
    rd_paint(&background, 0xff000064);
    rd_blend(&drm, &background);
    drm.pixels = drm_refresh();

    mouse.pixels = malloc(10 * 10 * 4);
    mouse.width = 10;
    mouse.height = 10;
    mouse.x = -mouse.width;
    mouse.y = -mouse.height;
    rd_paint(&mouse, 0xffffffff);

    char *mouse_event = "/dev/input/event2";
    int mouse_fd = open(mouse_event, O_RDONLY | O_CLOEXEC | O_NONBLOCK);

    int nop = 0;
    wl_event_loop_add_fd(loop, mouse_fd, WL_EVENT_READABLE, on_mouse_fd, &nop);

    fprintf(stderr, "Wayland compositor running on %s\n", socket);
    wl_display_run(wl_display);

    return 0;
}
