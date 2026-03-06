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

#include "types.h"

int fd;
uint32_t fb_ids[2] = {0};
uint32_t *fbs[2] = {0};
uint32_t active_fb = 0;
uint32_t inactive_fb = 1;

uint32_t crtc_id;
uint32_t conn_id;
drmModeModeInfo mode;

// For restoring the old display
drmModeCrtc *orig_crtc;

void drm_close() {
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

uint32_t *drm_refresh() {
    // Flip buffers
    active_fb = active_fb ^ 1;
    inactive_fb = inactive_fb ^ 1;

    // Display the activated buffer
    drmModeSetCrtc(fd, crtc_id, fb_ids[active_fb], 0, 0, &conn_id, 1, &mode);

    // Return the inactivated buffer for the compositor to work on.
    return fbs[inactive_fb];
}

int create_buffer(uint32_t fb_index, uint32_t width, uint32_t height) {
    // FB #1
    // Create a dumb buffer
    struct drm_mode_create_dumb cdumb = {0};
    cdumb.width  = width;
    cdumb.height = height;
    cdumb.bpp    = 32; // ARGB8888

    int error = drmIoctl(fd, DRM_IOCTL_MODE_CREATE_DUMB, &cdumb) < 0;
    if (error) {
        perror("CREATE_DUMB");
        return 1;
    }

    // Register the dumb buffer as a framebuffer object.    
    error = drmModeAddFB(fd, cdumb.width, cdumb.height, 24, 32, cdumb.pitch, cdumb.handle, &fb_ids[fb_index]);
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

    // return (uint32_t *)mmap(NULL, cdumb.size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, mdumb.offset);
    fbs[fb_index] = mmap(NULL, cdumb.size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, mdumb.offset);
    if (fbs[0] == MAP_FAILED) {
        perror("mmap");
        return 1;
    }
}

int drm_init(struct pxgrid *pxgrid) {
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

    mode = conn->modes[0];  // choose the first mode (usually preferred)
    pxgrid->width = mode.hdisplay;
    pxgrid->height = mode.vdisplay;

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

    crtc_id = enc->crtc_id;
    drmModeFreeEncoder(enc);


    // Save the original CRTC before painting so we can restore it when the program exits
    orig_crtc = drmModeGetCrtc(fd, crtc_id);
    if (!orig_crtc) { perror("drmModeGetCrtc"); return 1; }

    create_buffer(active_fb, pxgrid->width, pxgrid->height);
    create_buffer(inactive_fb, pxgrid->width, pxgrid->height);

    int error = drmModeSetCrtc(fd, crtc_id, fb_ids[active_fb], 0, 0, &conn_id, 1, &mode);
    if (error) {
        perror("drmModeSetCrtc");
        return 1;
    }

    pxgrid->pixels = fbs[inactive_fb];

    return 0;
}
