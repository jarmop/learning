#include <linux/input.h>
#include <stdint.h>
#include "types.h"

void drm_close();
// int drm_move_mouse(struct input_event ev);
// int drm_add_pixels(uint32_t *pixels, uint32_t width, uint32_t height, int x, int y);
void drm_refresh();
int drm_init(struct pxgrid *pxgrid);