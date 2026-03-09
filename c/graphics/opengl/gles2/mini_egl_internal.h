#ifndef MINI_EGL_INTERNAL_H
#define MINI_EGL_INTERNAL_H

#include "mini_backend.h"
#include "mini_driver_hook.h"
#include <gbm.h>

struct mini_display {
    struct gbm_device *gbm;
    int drm_fd;
    int initialized;

    const struct mini_backend *backend;
    const struct mini_driver_hook *driver_hook;

    void *backend_data;
    void *driver_data;

    struct mini_config default_config;
};

struct mini_context {
    struct mini_display *dpy;
    struct mini_config cfg;
    void *backend_data;
    void *driver_data;
};

struct mini_surface {
    struct mini_display *dpy;
    struct mini_config cfg;
    struct gbm_surface *gbm_surface;

    int width;
    int height;
    uint32_t format;

    struct gbm_bo *front_bo;
    int swapped;

    void *backend_data;
    void *driver_data;
};

struct mini_current {
    struct mini_display *dpy;
    struct mini_context *ctx;
    struct mini_surface *draw;
    struct mini_surface *read;
};

extern struct mini_current g_mini_current;

#endif