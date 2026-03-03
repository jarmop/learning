#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/mman.h>
#include <linux/input.h>
#include <xf86drm.h>
#include <xf86drmMode.h>

int fd;
uint32_t conn_id;
drmModeCrtc *orig_crtc;
struct drm_mode_create_dumb cdumb = {0};
drmModeModeInfo mode;
uint32_t crtc_id;
uint8_t *bitmap;
uint32_t fb_id;

void drawPlain(uint16_t width, uint16_t height, uint8_t *map, uint32_t pitch) {
    uint8_t r = 0; uint8_t g = 0; uint8_t b = 100;
    // Combine rgb values into one 3 byte value by shifting r bitwise to the 
    // left by 2 bytes, and g by 1 byte, and then doing bitwise or for each
    uint32_t pixel = (r << 16) | (g <<  8) | b;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            memcpy(map + y * pitch + x * 4, &pixel, 4);
        }
    }
}

void drawMouse(uint16_t mouse_x, uint16_t mouse_y, uint8_t *map, uint32_t pitch) {
    uint16_t mouse_width = 10; uint16_t mouse_height = 10;
    uint8_t r = 255; uint8_t g = 255; uint8_t b = 255;
    // Combine rgb values into one 3 byte value by shifting r bitwise to the 
    // left by 2 bytes, and g by 1 byte, and then doing bitwise or for each
    uint32_t pixel = (r << 16) | (g <<  8) | b;
    for (int y = mouse_y; y < mouse_y + mouse_height; y++) {
        for (int x = mouse_x; x < mouse_x + mouse_width; x++) {
            memcpy(map + y * pitch + x * 4, &pixel, 4);
        }
    }
}

void close_display() {
    // Restore the original CRTC
    drmModeSetCrtc(
        fd,
        orig_crtc->crtc_id,
        orig_crtc->buffer_id,
        orig_crtc->x,
        orig_crtc->y,
        &conn_id,
        1,
        &orig_crtc->mode
    );
    // drmModeFreeCrtc(orig_crtc);

    // Cleanup: unmap, rmfb, destroy dumb buffer
    // munmap(bitmap, cdumb.size);
    // drmModeRmFB(fd, fb_id);

    // struct drm_mode_destroy_dumb ddumb = {0};
    // ddumb.handle = cdumb.handle;
    // drmIoctl(fd, DRM_IOCTL_MODE_DESTROY_DUMB, &ddumb);

    // drmModeFreeResources(res);
    // close(fd);
}

int max_abs_x = 65482; int max_abs_y = 65452;
int mouse_x = -1; int mouse_y = -1;

int show_mouse_move(struct input_event ev) {
    if (ev.code == REL_X) {
        mouse_x = ev.value * cdumb.width / max_abs_x;
    } else if (ev.code == REL_Y) {
        mouse_y = ev.value * cdumb.height / max_abs_y;
    }
    // fprintf(stderr, "draw: %d\n", mouse_x > 0 && mouse_x < cdumb.width - 10 && mouse_y > 0 && mouse_y < cdumb.height - 10);
    // fprintf(stderr, "mouse_x:%d, cdumb_width: %d, mouse_y: %d, cdumb_height: %d\n", mouse_x, cdumb.width - 10, mouse_y, cdumb.height - 10);

    if (mouse_x > 0 && mouse_x < cdumb.width - 10 && mouse_y > 0 && mouse_y < cdumb.height - 10) {
        // fprintf(stderr, "Draw mouse\n");
        drawPlain(mode.hdisplay, mode.vdisplay, bitmap, cdumb.pitch);
        drawMouse(mouse_x, mouse_y, bitmap, cdumb.pitch);
    }

    drmModeDirtyFB(fd, fb_id, NULL, 0);

    return 0;
}

int initialize_display() {
    int mode_i = 0;
    // int screen_width = 925; int screen_height = 791;
    // int screen_width = 925; int screen_height = 1105;

    char *drm_device = "/dev/dri/card1";

    // Open the file representing the DRM device
    // CLOEXEC = close on exec
    fd = open(drm_device, O_RDWR | O_CLOEXEC);
    if (fd < 0) {
        perror(drm_device);
        return 1;
    }

    // Queries the kernel for all display-related objects
    drmModeRes *res = drmModeGetResources(fd);
    if (!res) {
        perror("drmModeGetResources");
        return 1;
    }

    drmModeConnector *conn = NULL;
    conn_id = 0;

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

    mode = conn->modes[mode_i];  // choose the first mode (usually preferred)
    // fprintf(stderr, "Available modes:\n");
    // for (int i = 0; i < conn->count_modes; i++) {
    //     fprintf(stderr, "  %d: %s\n", i, conn->modes[i].name);
    // }
    // fprintf(stderr, "Using mode %d: %s\n", mode_i, mode.name);
    // fprintf(stderr, "total – h: %d, v: %d\n", mode.htotal, mode.vtotal);
    // fprintf(stderr, "sync – h: %d-%d, v: %d-%d\n", mode.hsync_start, mode.hsync_end, mode.vsync_start, mode.vsync_end);
    // fprintf(stderr, "hskew: %d, vscan: %d\n", mode.hskew, mode.vscan);
    // fprintf(stderr, "clock: %d, flags: %x, type: %x\n", mode.clock, mode.flags, mode.type);

    // Find an encoder + CRTC
    drmModeEncoder *enc = drmModeGetEncoder(fd, conn->encoder_id);
    drmModeFreeConnector(conn);
    if (!enc) {
        fprintf(stderr, "No encoder for connector\n");
        return 1;
    }

    crtc_id = enc->crtc_id;
    drmModeFreeEncoder(enc);


    // Save the original CRTC before painting so we can restore it when the program exits
    orig_crtc = drmModeGetCrtc(fd, crtc_id);
    if (!orig_crtc) { perror("drmModeGetCrtc"); return 1; }

    // Create a dumb buffer
    // cdumb = {0};
    cdumb.width  = mode.hdisplay;
    cdumb.height = mode.vdisplay;
    cdumb.bpp    = 32; // ARGB8888

    int error = drmIoctl(fd, DRM_IOCTL_MODE_CREATE_DUMB, &cdumb) < 0;
    if (error) {
        perror("CREATE_DUMB");
        return 1;
    }

    // fprintf(stderr, "Screen width in pixels: %d \n", cdumb.width);
    // fprintf(stderr, "Screen height in pixels: %d \n", cdumb.height);
    // fprintf(stderr, "Bytes per pixel: %d \n", cdumb.bpp / 8);
    // fprintf(stderr, "Screen size in bytes: %lld \n", cdumb.size);
    // fprintf(stderr, "Pitch: %d \n", cdumb.pitch);

    // Register the dumb buffer as a framebuffer object.    
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

    bitmap = mmap(NULL, cdumb.size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, mdumb.offset);
    if (bitmap == MAP_FAILED) {
        perror("mmap");
        return 1;
    }

    // drawGradient(mode.hdisplay, mode.vdisplay, map, cdumb.pitch);
    // drawCheckers(mode.hdisplay, mode.vdisplay, map, cdumb.pitch);
    drawPlain(mode.hdisplay, mode.vdisplay, bitmap, cdumb.pitch);
    error = drmModeSetCrtc(fd, crtc_id, fb_id, 0, 0, &conn_id, 1, &mode);
    if (error) {
        perror("drmModeSetCrtc");
        return 1;
    }

    return 0;
}
