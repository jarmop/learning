#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    Display *display = XOpenDisplay(NULL);
    if (!display) {
        fprintf(stderr, "Cannot open display\n");
        return 1;
    }

    int screen = DefaultScreen(display);
    Window root = RootWindow(display, screen);

    Window win = XCreateSimpleWindow(
        display, root,
        100, 100, 400, 200, 1,
        BlackPixel(display, screen),
        WhitePixel(display, screen)
    );

    XStoreName(display, win, "Hello Window");

    // Select input events
    XSelectInput(display, win, ExposureMask | KeyPressMask);

    XMapWindow(display, win);

    GC gc = XCreateGC(display, win, 0, NULL);

    // Main event loop
    XEvent event;
    int running = 1;
    while (running) {
        XNextEvent(display, &event);
        if (event.type == Expose) {
            XDrawString(display, win, gc, 20, 50, "Hello World!", strlen("Hello World!"));
        }
        if (event.type == KeyPress) {
            running = 0;  // exit when any key is pressed
        }
    }

    XCloseDisplay(display);
    return 0;
}
