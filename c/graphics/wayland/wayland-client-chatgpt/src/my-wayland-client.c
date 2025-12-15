#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <errno.h>

#include <stdbool.h>

#include "wayland-example.h"

static int connect_to_socket(const char *name) {
	struct sockaddr_un addr;
	socklen_t size;
	const char *runtime_dir;
	int name_size, fd;

	runtime_dir = getenv("XDG_RUNTIME_DIR");

    // fprintf(stderr, "runtime_dir: %s", runtime_dir);

	fd = socket(PF_LOCAL, SOCK_STREAM | SOCK_CLOEXEC, 0);

	if (fd < 0) {
		return -1;
    }

	memset(&addr, 0, sizeof addr);
	addr.sun_family = AF_LOCAL;
    name_size = snprintf(
            addr.sun_path, 
            sizeof addr.sun_path, 
            "%s/%s", runtime_dir, name
        ) + 1;

	size = offsetof (struct sockaddr_un, sun_path) + name_size;

	if (connect(fd, (struct sockaddr *) &addr, size) < 0) {
		close(fd);
		return -1;
	}

	return fd;
}


/** Connect to a Wayland display
 *
 * \param name Name of the Wayland display to connect to
 * \return A wl_display object or NULL on failure
 */
struct wl_display * my_wl_display_connect(const char *name) {
	// char *connection, *end;
	int flags, fd;

	name = getenv("WAYLAND_DISPLAY");
    fprintf(stderr, "env name: %s\n", name);


	if (name == NULL) {
		name = "wayland-0";
	    fprintf(stderr, "using default name: %s\n", name);
	}


    fd = connect_to_socket(name);
    if (fd < 0) {
        return NULL;
    }

	return wl_display_connect_to_fd(fd);
}
