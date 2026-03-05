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
uint32_t fb_id;

// For restoring the old display
uint32_t conn_id;
drmModeCrtc *orig_crtc;

uint16_t display_width = 0;
uint16_t display_height = 0;

uint32_t *bitmap;
uint32_t *background;
uint32_t *mouse;

static void render_plain(uint32_t *pixels, uint16_t width, uint16_t height, uint32_t pixel) {
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            memcpy(pixels + y * width + x, &pixel, 4);
        }
    }
} 

static void render_background() {
    uint32_t r = 0; uint32_t g = 0; uint32_t b = 100;
    // Combine rgb values into one 3 byte value by shifting r bitwise to the 
    // left by 2 bytes, and g by 1 byte, and then doing bitwise or for each
    uint32_t pixel = (r << 16) | (g << 8) | b;
    render_plain(background, display_width, display_height, pixel);
}

static void render_mouse() {
    uint16_t mouse_width = 10; uint16_t mouse_height = 10;
    uint32_t r = 255; uint32_t g = 255; uint32_t b = 255;
    // Combine rgb values into one 3 byte value by shifting r bitwise to the 
    // left by 2 bytes, and g by 1 byte, and then doing bitwise or for each
    uint32_t pixel = (r << 16) | (g <<  8) | b;
    render_plain(mouse, mouse_width, mouse_height, pixel);
}

int display_add_pixels(uint32_t *pixels, uint32_t width, uint32_t height, int start_x, int start_y) {
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            uint32_t *pixel = pixels + y * width + x;
            int by = y + start_y;
            int bx = x + start_x;
            memcpy(bitmap + by * display_width + bx, pixel, 4);
        }
    }

    drmModeDirtyFB(fd, fb_id, NULL, 0);

    return 0;
}

void display_close() {
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

static int max_abs_x = 65482; int max_abs_y = 65452;
static int mouse_x = -1; int mouse_y = -1;

int display_move_mouse(struct input_event ev) {
    if (ev.code == REL_X) {
        mouse_x = ev.value * display_width / max_abs_x;
    } else if (ev.code == REL_Y) {
        mouse_y = ev.value * display_height / max_abs_y;
    }

    if (mouse_x > 0 && mouse_x < display_width - 10 && mouse_y > 0 && mouse_y < display_height - 10) {
        display_add_pixels(background, display_width, display_height, 0, 0);
        display_add_pixels(mouse, 10, 10, mouse_x, mouse_y);
    }

    drmModeDirtyFB(fd, fb_id, NULL, 0);

    return 0;
}

int display_init() {
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

    drmModeModeInfo mode = conn->modes[0];  // choose the first mode (usually preferred)
    display_width = mode.hdisplay;
    display_height = mode.vdisplay;
    // fprintf(stderr, "Available modes:\n");
    // for (int i = 0; i < conn->count_modes; i++) {
    //     fprintf(stderr, "  %d: %s\n", i, conn->modes[i].name);
    // }
    // fprintf(stderr, "Using mode %s\n", mode.name);
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

    uint32_t crtc_id = enc->crtc_id;
    drmModeFreeEncoder(enc);


    // Save the original CRTC before painting so we can restore it when the program exits
    orig_crtc = drmModeGetCrtc(fd, crtc_id);
    if (!orig_crtc) { perror("drmModeGetCrtc"); return 1; }

    // Create a dumb buffer
    struct drm_mode_create_dumb cdumb = {0};
    cdumb.width  = display_width;
    cdumb.height = display_height;
    cdumb.bpp    = 32; // ARGB8888

    int error = drmIoctl(fd, DRM_IOCTL_MODE_CREATE_DUMB, &cdumb) < 0;
    if (error) {
        perror("CREATE_DUMB");
        return 1;
    }

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

    background = malloc(display_width * display_height * 4);
    mouse = malloc(10 * 10 * 4);
    render_background();
    render_mouse();
    display_add_pixels(background, display_width, display_height, 0, 0);

    error = drmModeSetCrtc(fd, crtc_id, fb_id, 0, 0, &conn_id, 1, &mode);
    if (error) {
        perror("drmModeSetCrtc");
        return 1;
    }

    return 0;
}
