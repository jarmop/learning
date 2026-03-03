#include <linux/input.h>

void close_display();
int show_mouse_move(struct input_event ev);
int initialize_display();