#include <linux/input.h>
#include <stdint.h>

void close_display();
int show_mouse_move(struct input_event ev);
int show_surface(uint32_t *pixels, size_t size, uint32_t stride, int x, int y);
int initialize_display();