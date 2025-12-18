#include <stdio.h>
#include <wayland-server.h>

// struct my_state {
//     /* Globals */
//     struct wl_display *wl_display;
//     struct wl_registry *wl_registry;
//     struct wl_shm *wl_shm;
//     struct wl_compositor *wl_compositor;
//     struct xdg_wm_base *xdg_wm_base;
//     /* Objects */
//     struct wl_surface *wl_surface;
//     struct xdg_surface *xdg_surface;
//     struct xdg_toplevel *xdg_toplevel;
// };

struct my_state {
    /* Globals */
    struct wl_display *wl_display;
};

static void wl_output_handle_bind(
    struct wl_client *client, 
    void *data,
    uint32_t version, 
    uint32_t id
) {
    fprintf(stderr, "Binding to wl_output.\n");
    struct my_state *state = data;
    // TODO
}

int main(int argc, char *argv[]) {
    struct wl_display *display = wl_display_create();
    if (!display) {
        fprintf(stderr, "Unable to create Wayland display.\n");
        return 1;
    }

    struct my_state state = { wl_display: display };

    wl_global_create(display, &wl_output_interface,
        1, &state, wl_output_handle_bind);

    const char *socket = wl_display_add_socket_auto(display);
    if (!socket) {
        fprintf(stderr, "Unable to add socket to Wayland display.\n");
        return 1;
    }

    fprintf(stderr, "Running Wayland display on %s\n", socket);
    wl_display_run(display);

    wl_display_destroy(display);
    return 0;
}