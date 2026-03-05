#include <linux/input.h>
#include <stdint.h>

void display_close();
int display_move_mouse(struct input_event ev);
int display_add_pixels(uint32_t *pixels, uint32_t width, uint32_t height, int x, int y);
int display_init();