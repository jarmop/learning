// #include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include <unistd.h>
// #include <stdint.h>
// #include <fcntl.h>
#include <errno.h>
#include <sys/mman.h>
#include <linux/input.h>

#include <xf86drm.h>
#include <xf86drmMode.h>

#include "drm-demo.h"

int main() {
    // alpine
    char mouse_event[] = "/dev/input/event3";
    int mode_i = 12;
    int screen_width = 800; int screen_height = 600;
    int max_abs_x = 65452; int max_abs_y = 65424;

    // ubuntu
    // char mouse_event[] = "/dev/input/event2";
    // int mode_i = 0;
    // int screen_width = 1280; int screen_height = 800;
    // int max_abs_x = 65482; int max_abs_y = 65452;

    // Open the file representing the DRM device
    // CLOEXEC = close on exec
    int fd = open("/dev/dri/card0", O_RDWR | O_CLOEXEC);
    if (fd < 0) {
        perror("open /dev/dri/card0");
        return 1;
    }

    // Queries the kernel for all display-related objects
    drmModeRes *res = drmModeGetResources(fd);
    if (!res) {
        perror("drmModeGetResources");
        return 1;
    }

    drmModeConnector *conn = NULL;
    uint32_t conn_id = 0;

    // Find the first connected connector
    for (int i = 0; i < res->count_connectors; i++) {
        conn = drmModeGetConnector(fd, res->connectors[i]);
        if (conn->connection == DRM_MODE_CONNECTED && conn->count_modes > 0) {
            conn_id = conn->connector_id;
            break;
        }
        drmModeFreeConnector(conn);
        conn = NULL;
    }

    if (!conn) {
        fprintf(stderr, "No connected monitor!\n");
        return 1;
    }


    drmModeModeInfo mode = conn->modes[mode_i];  // choose first mode (usually preferred)
    fprintf(stderr, "Available modes:\n");
    for (int i = 0; i < conn->count_modes; i++) {
        fprintf(stderr, "  %d: %s\n", i, conn->modes[i].name);
    }
    fprintf(stderr, "Using mode %d: %s\n", mode_i, mode.name);

    // Find an encoder + CRTC
    drmModeEncoder *enc = drmModeGetEncoder(fd, conn->encoder_id);
    if (!enc) {
        fprintf(stderr, "No encoder for connector\n");
        return 1;
    }

    uint32_t crtc_id = enc->crtc_id;
    drmModeFreeEncoder(enc);

    // Create a dumb buffer
    struct drm_mode_create_dumb cdumb = {0};
    cdumb.width  = mode.hdisplay;
    cdumb.height = mode.vdisplay;
    cdumb.bpp    = 32; // ARGB8888

    int error = drmIoctl(fd, DRM_IOCTL_MODE_CREATE_DUMB, &cdumb) < 0;
    if (error) {
        perror("CREATE_DUMB");
        return 1;
    }

    fprintf(stderr, "Screen width in pixels: %d \n", cdumb.width);
    fprintf(stderr, "Screen height in pixels: %d \n", cdumb.height);
    fprintf(stderr, "Bytes per pixel: %d \n", cdumb.bpp / 8);
    fprintf(stderr, "Screen size in bytes: %lld \n", cdumb.size);
    fprintf(stderr, "Pitch: %d \n", cdumb.pitch);

    // Register the dumb buffer as a framebuffer object.
    uint32_t fb_id;
    error = drmModeAddFB(fd, cdumb.width, cdumb.height, 24, 32, cdumb.pitch, cdumb.handle, &fb_id);
    if (error) {
        perror("drmModeAddFB");
        return 1;
    }

    // Map the buffer so we can draw into it
    struct drm_mode_map_dumb mdumb = {0};
    mdumb.handle = cdumb.handle;
    error = drmIoctl(fd, DRM_IOCTL_MODE_MAP_DUMB, &mdumb);
    if (error) {
        perror("MAP_DUMB");
        return 1;
    }

    uint8_t *map = mmap(NULL, cdumb.size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, mdumb.offset);
    if (map == MAP_FAILED) {
        perror("mmap");
        return 1;
    }

    // drawGradient(mode.hdisplay, mode.vdisplay, map, cdumb.pitch);
    // drawCheckers(mode.hdisplay, mode.vdisplay, map, cdumb.pitch);

    int mouse_fd = open(mouse_event, O_RDONLY | O_CLOEXEC);

    fprintf(stderr, "Mousefile: %s \n", mouse_event);

    struct input_event ev;
    int mouse_x = -1; int mouse_y = -1;
    
    int track_mouse = 1;
    while (track_mouse) {
        read(mouse_fd, &ev, sizeof(ev));

        if (ev.type == EV_REL) {
            if (ev.code == REL_X) mouse_x += ev.value;
            if (ev.code == REL_Y) mouse_y += ev.value;
            fprintf(stderr, "mouse x: %d, y: %d\n", mouse_x, mouse_y);
        }

        if (ev.type == EV_ABS) {
            if (ev.code == REL_X) {
                mouse_x = ev.value * screen_width / max_abs_x;
                fprintf(stderr, "ev.value x: %d\n", ev.value);
            }
            if (ev.code == REL_Y) { 
                mouse_y = ev.value * screen_height / max_abs_y; 
                fprintf(stderr, "ev.value x: %d\n", ev.value);
            }
            fprintf(stderr, "mouse x: %d, y: %d\n", mouse_x, mouse_y);
        }

        if (ev.type == EV_KEY && ev.code == BTN_LEFT) {
            if (ev.value == 1) printf("Left button pressed\n");
            if (ev.value == 0) {
                printf("Left button released\n");
                track_mouse = 0;
            }
        }

        // Redraw cursor here
        if (ev.type == EV_ABS && mouse_x > 0 && mouse_x < cdumb.width - 10 && mouse_y > 0 && mouse_y < cdumb.height - 10) {
            fprintf(stderr, "Draw mouse\n");
            drawPlain(mode.hdisplay, mode.vdisplay, map, cdumb.pitch);
            drawMouse(mouse_x, mouse_y, map, cdumb.pitch);
        }

        // Display it with the KMS modeset call
        error = drmModeSetCrtc(fd, crtc_id, fb_id, 0, 0, &conn_id, 1, &mode);
        if (error) {
            perror("drmModeSetCrtc");
            return 1;
        }
    }

    // mouse();

    // sleep(5);

    // Cleanup: unmap, rmfb, destroy dumb buffer
    munmap(map, cdumb.size);
    drmModeRmFB(fd, fb_id);

    struct drm_mode_destroy_dumb ddumb = {0};
    ddumb.handle = cdumb.handle;
    drmIoctl(fd, DRM_IOCTL_MODE_DESTROY_DUMB, &ddumb);

    drmModeFreeConnector(conn);
    drmModeFreeResources(res);
    close(fd);

    return 0;
}
