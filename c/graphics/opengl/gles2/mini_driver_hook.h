#ifndef MINI_DRIVER_HOOK_H
#define MINI_DRIVER_HOOK_H

struct mini_display;
struct mini_context;
struct mini_surface;

struct mini_driver_hook_vtbl {
    int  (*init_driver)(struct mini_display *dpy);
    void (*shutdown_driver)(struct mini_display *dpy);

    int  (*create_context)(struct mini_context *ctx);
    void (*destroy_context)(struct mini_context *ctx);

    int  (*create_drawable)(struct mini_surface *surf);
    void (*destroy_drawable)(struct mini_surface *surf);

    int  (*bind_current)(
        struct mini_display *dpy,
        struct mini_surface *draw,
        struct mini_surface *read,
        struct mini_context *ctx
    );

    int  (*unbind_current)(struct mini_display *dpy);

    int  (*flush_and_publish)(struct mini_display *dpy, struct mini_surface *surf);
};

struct mini_driver_hook {
    const char *name;
    const struct mini_driver_hook_vtbl *vtbl;
};

extern const struct mini_driver_hook mini_driver_hook_egl;

#endif