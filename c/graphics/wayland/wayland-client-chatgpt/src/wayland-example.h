#include <wayland-client.h>

struct wl_display * my_wl_display_connect(const char *name);
// int my_wl_display_dispatch(struct wl_display *display);
// struct wl_registry * my_wl_display_get_registry(struct wl_display *wl_display);

/**
 * @ingroup iface_wl_display
 *
 * This request creates a registry object that allows the client
 * to list and bind the global objects available from the
 * compositor.
 */
static inline struct wl_registry * my_wl_display_get_registry(struct wl_display *wl_display)
{
	struct wl_proxy *registry;

	registry = wl_proxy_marshal_flags((struct wl_proxy *) wl_display,
			 WL_DISPLAY_GET_REGISTRY, &wl_registry_interface, wl_proxy_get_version((struct wl_proxy *) wl_display), 0, NULL);

	return (struct wl_registry *) registry;
}