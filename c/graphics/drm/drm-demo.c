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

int main() {
    int fd = open("/dev/dri/card0", O_RDWR | O_CLOEXEC);
    if (fd < 0) {
        perror("open /dev/dri/card0");
        return 1;
    }

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

    drmModeModeInfo mode = conn->modes[0];  // choose first mode (usually preferred)
    printf("Using mode: %s %dx%d\n",
           mode.name, mode.hdisplay, mode.vdisplay);

    // Find an encoder + CRTC
    drmModeEncoder *enc = drmModeGetEncoder(fd, conn->encoder_id);
    if (!enc) {
        fprintf(stderr, "No encoder for connector\n");
        return 1;
    }

    uint32_t crtc_id = enc->crtc_id;
    drmModeFreeEncoder(enc);

    // Create a dumb buffer
    struct drm_mode_create_dumb creq = {0};
    creq.width  = mode.hdisplay;
    creq.height = mode.vdisplay;
    creq.bpp    = 32; // ARGB8888

    if (drmIoctl(fd, DRM_IOCTL_MODE_CREATE_DUMB, &creq) < 0) {
        perror("CREATE_DUMB");
        return 1;
    }

    uint32_t handle = creq.handle;
    uint32_t pitch  = creq.pitch;
    uint64_t screensize   = creq.size;

    // Prepare framebuffer object
    uint32_t fb_id;
    if (drmModeAddFB(fd, creq.width, creq.height, 24, 32, pitch, handle, &fb_id)) {
        perror("drmModeAddFB");
        return 1;
    }

    // Map it
    struct drm_mode_map_dumb mreq = {0};
    mreq.handle = handle;

    if (drmIoctl(fd, DRM_IOCTL_MODE_MAP_DUMB, &mreq)) {
        perror("MAP_DUMB");
        return 1;
    }

    uint8_t *map = mmap(NULL, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, mreq.offset);
    if (map == MAP_FAILED) {
        perror("mmap");
        return 1;
    }

    // Draw a simple gradient
    for (int y = 0; y < mode.vdisplay; y++) {
        for (int x = 0; x < mode.hdisplay; x++) {
            uint8_t r = (x * 255) / mode.hdisplay;
            uint8_t g = (y * 255) / mode.vdisplay;
            uint8_t b = 128;

            uint32_t pixel =
                (r << 16) |   // R
                (g <<  8) |   // G
                (b <<  0);    // B

            memcpy(map + y * pitch + x * 4, &pixel, 4);
        }
    }

    // Display it
    if (drmModeSetCrtc(fd, crtc_id, fb_id, 0, 0,
                       &conn_id, 1, &mode)) {
        perror("drmModeSetCrtc");
        return 1;
    }

    printf("Gradient displayed! Sleeping 5 seconds…\n");
    sleep(5);

    // Cleanup: unmap, rmfb, destroy dumb buffer
    munmap(map, screensize);
    drmModeRmFB(fd, fb_id);

    struct drm_mode_destroy_dumb dreq = {0};
    dreq.handle = handle;
    drmIoctl(fd, DRM_IOCTL_MODE_DESTROY_DUMB, &dreq);

    drmModeFreeConnector(conn);
    drmModeFreeResources(res);
    close(fd);

    return 0;
}
