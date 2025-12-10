#include <linux/input.h>

#include "drm-demo.h"

void mouse() {
    int fd = open("/dev/input/event3", O_RDONLY | O_CLOEXEC);
    struct input_event ev;
    int mouse_x = 0; int mouse_y = 0;
    
    // evtest Max 65535
    // int screen_width = 1200; int screen_height = 800;
    // int max_abs_x = 65482; int max_abs_y = 65452;
    int screen_width = 800; int screen_height = 600;
    int max_abs_x = 65452; int max_abs_y = 65424;

    while (1) {
        read(fd, &ev, sizeof(ev));

        if (ev.type == EV_REL) {
            if (ev.code == REL_X) mouse_x += ev.value;
            if (ev.code == REL_Y) mouse_y += ev.value;
            fprintf(stderr, "mouse x: %d, y: %d\n", mouse_x, mouse_y);
        }

        if (ev.type == EV_ABS) {
            if (ev.code == REL_X) mouse_x = ev.value * screen_width / max_abs_x;
            if (ev.code == REL_Y) mouse_y = ev.value * screen_height / max_abs_y;
            fprintf(stderr, "mouse x: %d, y: %d\n", mouse_x, mouse_y);
        }

        if (ev.type == EV_KEY && ev.code == BTN_LEFT) {
            if (ev.value == 1) printf("Left button pressed\n");
            if (ev.value == 0) printf("Left button released\n");
        }

        // Redraw cursor here
    }
}
