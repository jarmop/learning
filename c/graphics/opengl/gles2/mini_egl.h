#ifndef MINI_EGL_H
#define MINI_EGL_H

#include <stdint.h>
#include <gbm.h>

#ifdef __cplusplus
extern "C" {
#endif

struct mini_display;
struct mini_config;
struct mini_context;
struct mini_surface;

enum mini_result {
    MINI_OK = 0,
    MINI_ERR_BAD_ARG = -1,
    MINI_ERR_BAD_STATE = -2,
    MINI_ERR_NOMEM = -3,
    MINI_ERR_UNSUPPORTED = -4,
};

struct mini_config {
    uint32_t gbm_format;
    int gles_version;
    int depth_bits;
    int stencil_bits;
    int has_alpha;
};

struct mini_display *miniGetDisplay(struct gbm_device *gbm, int drm_fd);
int miniInitialize(struct mini_display *dpy);
void miniTerminate(struct mini_display *dpy);

const struct mini_config *miniChooseConfig(struct mini_display *dpy);

struct mini_context *miniCreateContext(
    struct mini_display *dpy,
    const struct mini_config *cfg
);
void miniDestroyContext(struct mini_context *ctx);

struct mini_surface *miniCreateWindowSurface(
    struct mini_display *dpy,
    const struct mini_config *cfg,
    struct gbm_surface *gbm_surface,
    int width,
    int height
);
void miniDestroySurface(struct mini_surface *surf);

int miniMakeCurrent(
    struct mini_display *dpy,
    struct mini_surface *draw,
    struct mini_surface *read,
    struct mini_context *ctx
);

int miniSwapBuffers(struct mini_display *dpy, struct mini_surface *surf);

struct gbm_bo *miniLockFrontBuffer(struct mini_surface *surf);
void miniReleaseBuffer(struct mini_surface *surf, struct gbm_bo *bo);

#ifdef __cplusplus
}
#endif

#endif