// drm-minimal.c
// Minimal DRM/KMS dumb-buffer demo.
// Draws a color gradient on the primary monitor using direct modesetting.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/mman.h>

#include <xf86drm.h>
#include <xf86drmMode.h>

void drawGradient(uint16_t width, u_int16_t height, uint8_t *map, uint32_t pitch) {
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            uint8_t r = (x * 255) / width;
            uint8_t g = (y * 255) / height;
            uint8_t b = 128;

            // Combine rgb values into one 3 byte value by shifting r bitwise to the 
            // left by 2 bytes, and g by 1 byte, and then doing bitwise or for each
            uint32_t pixel = (r << 16) | (g <<  8) | b;

            memcpy(map + y * pitch + x * 4, &pixel, 4);
        }
    }
}

void drawCheckers(uint16_t width, u_int16_t height, uint8_t *map, uint32_t pitch) {
    int yinc = height / 10;
    int xinc = width / 10;
    for (int y = 0; y < height; y ++) {
        bool even_y = y/yinc%2 == 0;
        int scale_a, scale_b;
        if (even_y) {
            scale_a = 255; scale_b = 0;
        } else {
            scale_a = 0; scale_b = 255;
        }
        int scale_a = y/yinc%2 * 255;
        for (int x = 0; x < width; x ++) {
            bool even_x = x/xinc%2 == 0;
            int scale;
            if (even_x) {
                scale = scale_a;
            } else {
                scale = scale_b;
            }
            uint8_t r = scale;
            uint8_t g = scale;
            uint8_t b = scale;

            // Combine rgb values into one 3 byte value by shifting r bitwise to the 
            // left by 2 bytes, and g by 1 byte, and then doing bitwise or for each
            uint32_t pixel = (r << 16) | (g <<  8) | b;

            memcpy(map + y * pitch + x * 4, &pixel, 4);
        }
    }
}

int main() {
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

    int mode_i = 12;

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

    if (drmIoctl(fd, DRM_IOCTL_MODE_CREATE_DUMB, &cdumb) < 0) {
        perror("CREATE_DUMB");
        return 1;
    }

    fprintf(stderr, "Screen width in pixels: %d \n", cdumb.width);
    fprintf(stderr, "Screen height in pixels: %d \n", cdumb.height);
    fprintf(stderr, "Bytes per pixel: %d \n", cdumb.bpp / 8);
    fprintf(stderr, "Screen size in bytes: %lld \n", cdumb.size);

    // Register the dumb buffer as a framebuffer object.
    uint32_t fb_id;
    int error = drmModeAddFB(fd, cdumb.width, cdumb.height, 24, 32, cdumb.pitch, cdumb.handle, &fb_id);
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
    drawCheckers(mode.hdisplay, mode.vdisplay, map, cdumb.pitch);

    // Display it
    error = drmModeSetCrtc(fd, crtc_id, fb_id, 0, 0, &conn_id, 1, &mode);
    if (error) {
        perror("drmModeSetCrtc");
        return 1;
    }

    fprintf(stderr, "Gradient displayed! Sleeping 5 seconds…\n");
    sleep(5);

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
