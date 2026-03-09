#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

#include <xf86drm.h>
#include <xf86drmMode.h>
#include <gbm.h>

#include <GLES2/gl2.h>

#include "mini_egl.h"

static drmModeConnector *find_connector(int fd, drmModeRes *res)
{
    for (int i = 0; i < res->count_connectors; i++) {
        drmModeConnector *conn = drmModeGetConnector(fd, res->connectors[i]);
        if (conn->connection == DRM_MODE_CONNECTED && conn->count_modes > 0)
            return conn;
        drmModeFreeConnector(conn);
    }
    return NULL;
}

int main()
{
    int drm_fd = open("/dev/dri/card1", O_RDWR | O_CLOEXEC);

    drmModeRes *res = drmModeGetResources(drm_fd);
    drmModeConnector *conn = find_connector(drm_fd, res);

    drmModeModeInfo mode = conn->modes[0];

    drmModeEncoder *enc = drmModeGetEncoder(drm_fd, conn->encoder_id);
    uint32_t crtc = enc->crtc_id;

    struct gbm_device *gbm = gbm_create_device(drm_fd);

    struct mini_display *dpy = miniGetDisplay(gbm, drm_fd);
    miniInitialize(dpy);

    const struct mini_config *cfg = miniChooseConfig(dpy);

    struct mini_context *ctx = miniCreateContext(dpy, cfg);

    struct mini_surface *surf = miniCreateWindowSurface(dpy, cfg, mode.hdisplay, mode.vdisplay);

    miniMakeCurrent(dpy, surf, surf, ctx);

    glViewport(0, 0, mode.hdisplay, mode.vdisplay);
    glClearColor(0.2, 0.3, 0.8, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    miniSwapBuffers(dpy, surf);

    struct gbm_bo *bo = miniLockFrontBuffer(surf);
    if (!bo) {
        fprintf(stderr, "No front buffer available\n");
        return 1;
    }

    uint32_t fb;

    uint32_t handle = gbm_bo_get_handle(bo).u32;

    uint32_t stride = gbm_bo_get_stride(bo);

    drmModeAddFB(
        drm_fd,
        mode.hdisplay,
        mode.vdisplay,
        24,
        32,
        stride,
        handle,
        &fb
    );

    drmModeSetCrtc(
        drm_fd,
        crtc,
        fb,
        0,
        0,
        &conn->connector_id,
        1,
        &mode
    );

    sleep(3);

    miniReleaseBuffer(surf, bo);

    return 0;
}