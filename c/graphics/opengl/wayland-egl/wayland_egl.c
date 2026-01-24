#define _POSIX_C_SOURCE 200809L
#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <wayland-client.h>
#include <wayland-egl.h>

#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES2/gl2.h>

#include "xdg-shell-client-protocol.h"

struct app {
    struct wl_display *display;
    struct wl_registry *registry;
    struct wl_compositor *compositor;
    struct xdg_wm_base *wm_base;

    struct wl_surface *surface;
    struct xdg_surface *xdg_surface;
    struct xdg_toplevel *toplevel;

    struct wl_egl_window *egl_window;

    EGLDisplay egl_display;
    EGLConfig  egl_config;
    EGLContext egl_context;
    EGLSurface egl_surface;

    int width, height;
    bool running;
    bool configured;
};

static void die(const char *msg) {
    fprintf(stderr, "%s: %s\n", msg, strerror(errno));
    exit(1);
}

/* ---------- xdg-shell ---------- */

static void xdg_wm_base_ping(void *data, struct xdg_wm_base *wm_base, uint32_t serial) {
    (void)data;
    xdg_wm_base_pong(wm_base, serial);
}

static const struct xdg_wm_base_listener wm_base_listener = {
    .ping = xdg_wm_base_ping,
};

static void xdg_surface_configure(void *data, struct xdg_surface *xdg_surface, uint32_t serial) {
    struct app *a = data;
    xdg_surface_ack_configure(xdg_surface, serial);
    a->configured = true;
}

static const struct xdg_surface_listener xdg_surface_listener = {
    .configure = xdg_surface_configure,
};

static void toplevel_configure(void *data,
                              struct xdg_toplevel *toplevel,
                              int32_t width,
                              int32_t height,
                              struct wl_array *states) {
    (void)toplevel;
    (void)states;
    struct app *a = data;

    /* Compositors may send 0,0 meaning “no preference” */
    if (width > 0)  a->width = width;
    if (height > 0) a->height = height;

    if (a->egl_window) {
        wl_egl_window_resize(a->egl_window, a->width, a->height, 0, 0);
    }
}

static void toplevel_close(void *data, struct xdg_toplevel *toplevel) {
    (void)toplevel;
    struct app *a = data;
    a->running = false;
}

static const struct xdg_toplevel_listener toplevel_listener = {
    .configure = toplevel_configure,
    .close = toplevel_close,
};

/* ---------- registry ---------- */

static void registry_global(void *data,
                            struct wl_registry *registry,
                            uint32_t name,
                            const char *interface,
                            uint32_t version) {
    struct app *a = data;

    if (strcmp(interface, wl_compositor_interface.name) == 0) {
        a->compositor = wl_registry_bind(registry, name, &wl_compositor_interface, 4);
    } else if (strcmp(interface, xdg_wm_base_interface.name) == 0) {
        a->wm_base = wl_registry_bind(registry, name, &xdg_wm_base_interface, 1);
        xdg_wm_base_add_listener(a->wm_base, &wm_base_listener, a);
    }

    (void)version;
}

static void registry_remove(void *data, struct wl_registry *registry, uint32_t name) {
    (void)data; (void)registry; (void)name;
}

static const struct wl_registry_listener registry_listener = {
    .global = registry_global,
    .global_remove = registry_remove,
};

/* ---------- EGL/GLES ---------- */

static void init_egl(struct app *a) {
    a->egl_display = eglGetDisplay((EGLNativeDisplayType)a->display);
    if (a->egl_display == EGL_NO_DISPLAY) die("eglGetDisplay failed");

    if (!eglInitialize(a->egl_display, NULL, NULL)) die("eglInitialize failed");

    static const EGLint cfg_attribs[] = {
        EGL_SURFACE_TYPE,    EGL_WINDOW_BIT,
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
        EGL_RED_SIZE,        8,
        EGL_GREEN_SIZE,      8,
        EGL_BLUE_SIZE,       8,
        EGL_ALPHA_SIZE,      8,
        EGL_NONE
    };

    EGLint n = 0;
    if (!eglChooseConfig(a->egl_display, cfg_attribs, &a->egl_config, 1, &n) || n < 1) {
        die("eglChooseConfig failed");
    }

    static const EGLint ctx_attribs[] = {
        EGL_CONTEXT_CLIENT_VERSION, 2,
        EGL_NONE
    };

    a->egl_context = eglCreateContext(a->egl_display, a->egl_config, EGL_NO_CONTEXT, ctx_attribs);
    if (a->egl_context == EGL_NO_CONTEXT) die("eglCreateContext failed");
}

static void create_window_and_surface(struct app *a) {
    a->surface = wl_compositor_create_surface(a->compositor);
    if (!a->surface) die("wl_compositor_create_surface failed");

    a->xdg_surface = xdg_wm_base_get_xdg_surface(a->wm_base, a->surface);
    xdg_surface_add_listener(a->xdg_surface, &xdg_surface_listener, a);

    a->toplevel = xdg_surface_get_toplevel(a->xdg_surface);
    xdg_toplevel_set_title(a->toplevel, "Wayland + EGL Hello");
    xdg_toplevel_add_listener(a->toplevel, &toplevel_listener, a);

    wl_surface_commit(a->surface);

    /* Wait until first configure before creating wl_egl_window (recommended) */
    while (!a->configured) {
        if (wl_display_roundtrip(a->display) < 0) die("wl_display_roundtrip");
    }

    a->egl_window = wl_egl_window_create(a->surface, a->width, a->height);
    if (!a->egl_window) die("wl_egl_window_create failed");

    a->egl_surface = eglCreateWindowSurface(a->egl_display, a->egl_config,
                                           (EGLNativeWindowType)a->egl_window, NULL);
    if (a->egl_surface == EGL_NO_SURFACE) die("eglCreateWindowSurface failed");

    if (!eglMakeCurrent(a->egl_display, a->egl_surface, a->egl_surface, a->egl_context)) {
        die("eglMakeCurrent failed");
    }
}

static void draw_frame(struct app *a, float t) {
    glViewport(0, 0, a->width, a->height);

    /* Animate the clear color a bit so you can see frames */
    float r = 0.2f + 0.2f * (0.5f + 0.5f * sinf(t));
    float g = 0.2f;
    float b = 0.3f;

    glClearColor(r, g, b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    eglSwapBuffers(a->egl_display, a->egl_surface);
}

int main(void) {
    struct app a = {
        .width = 800,
        .height = 600,
        .running = true,
        .configured = false,
    };

    a.display = wl_display_connect(NULL);
    if (!a.display) die("wl_display_connect failed");

    a.registry = wl_display_get_registry(a.display);
    wl_registry_add_listener(a.registry, &registry_listener, &a);

    /* Gather globals */
    if (wl_display_roundtrip(a.display) < 0) die("wl_display_roundtrip");

    if (!a.compositor || !a.wm_base) {
        fprintf(stderr, "Missing wl_compositor or xdg_wm_base (are you on a Wayland session?)\n");
        return 1;
    }

    init_egl(&a);
    create_window_and_surface(&a);

    float t = 0.0f;
    while (a.running) {
        /* Pump events without blocking too long */
        wl_display_dispatch_pending(a.display);
        wl_display_flush(a.display);

        draw_frame(&a, t);
        t += 0.016f; /* ~60fps-ish */
    }

    /* Cleanup (minimal) */
    if (a.egl_display != EGL_NO_DISPLAY) {
        eglMakeCurrent(a.egl_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        if (a.egl_surface) eglDestroySurface(a.egl_display, a.egl_surface);
        if (a.egl_context) eglDestroyContext(a.egl_display, a.egl_context);
        eglTerminate(a.egl_display);
    }

    if (a.egl_window) wl_egl_window_destroy(a.egl_window);
    if (a.toplevel) xdg_toplevel_destroy(a.toplevel);
    if (a.xdg_surface) xdg_surface_destroy(a.xdg_surface);
    if (a.surface) wl_surface_destroy(a.surface);

    if (a.wm_base) xdg_wm_base_destroy(a.wm_base);
    if (a.compositor) wl_compositor_destroy(a.compositor);
    if (a.registry) wl_registry_destroy(a.registry);

    wl_display_disconnect(a.display);
    return 0;
}
