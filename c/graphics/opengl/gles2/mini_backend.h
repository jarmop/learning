#ifndef MINI_BACKEND_H
#define MINI_BACKEND_H

#include "mini_egl.h"

struct mini_backend;
struct mini_display;
struct mini_context;
struct mini_surface;

struct mini_backend_vtbl {
    int (*initialize)(struct mini_display *dpy);
    void (*terminate)(struct mini_display *dpy);

    int (*create_context)(struct mini_context *ctx);
    void (*destroy_context)(struct mini_context *ctx);

    int (*create_surface)(struct mini_surface *surf);
    void (*destroy_surface)(struct mini_surface *surf);

    int (*make_current)(
        struct mini_display *dpy,
        struct mini_surface *draw,
        struct mini_surface *read,
        struct mini_context *ctx
    );

    int (*swap_buffers)(struct mini_display *dpy, struct mini_surface *surf);
};

struct mini_backend {
    const char *name;
    const struct mini_backend_vtbl *vtbl;
};

extern const struct mini_backend mini_backend_custom;
extern const struct mini_backend mini_backend_egl;

#endif