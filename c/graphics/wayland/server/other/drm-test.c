// #include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
// #include <stdint.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/mman.h>
#include <linux/input.h>
#include <stdio.h>

#include <xf86drm.h>
#include <xf86drmMode.h>

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

int main() {
    // alpine
    // int mode_i = 12;
    // int screen_width = 800; int screen_height = 600;
    // int max_abs_x = 65452; int max_abs_y = 65424;



    // ubuntu
    int mode_i = 0;
    int screen_width = 1280; int screen_height = 800;
    int max_abs_x = 65482; int max_abs_y = 65452;

    // Open the file representing the DRM device
    // CLOEXEC = close on exec
    char drm_device_path[] = "/dev/dri/card0";
    int fd = open(drm_device_path, O_RDWR | O_CLOEXEC);
    if (fd < 0) {
        char error_header[50];
        sprintf(error_header, "open %s", drm_device_path);
        perror(error_header);
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

    fprintf(stderr, "count_connectors: %d\n", res->count_connectors);
    // return 0;

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

    // Store the current screen contents for repainting after the program exits
    drmModeCrtc *orig_crtc = drmModeGetCrtc(fd, crtc_id);

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

    drawPlain(mode.hdisplay, mode.vdisplay, map, cdumb.pitch);

    // Display it with the KMS modeset call
    error = drmModeSetCrtc(fd, crtc_id, fb_id, 0, 0, &conn_id, 1, &mode);
    if (error) {
        perror("drmModeSetCrtc");
        return 1;
    }

    sleep(5);

    // Restore the original screen contents
    drmModeSetCrtc(fd,
        orig_crtc->crtc_id,
        orig_crtc->buffer_id,
        orig_crtc->x,
        orig_crtc->y,
        &conn_id,
        1,
        &orig_crtc->mode);
    drmModeFreeCrtc(orig_crtc);

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
