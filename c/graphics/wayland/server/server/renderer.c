#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include "types.h"
#include "renderer.h"

void rd_paint(struct pxgrid *pxgrid, uint32_t color) {
    for (int y = 0; y < pxgrid->height; y++) {
        for (int x = 0; x < pxgrid->width; x++) {
            memcpy(pxgrid->pixels + y * pxgrid->width + x, &color, 4);
        }
    }
}

void rd_blend(struct pxgrid *back, struct pxgrid *front) {
    for (int y = 0; y < front->height; y++) {
        for (int x = 0; x < front->width; x++) {
            uint32_t *pixel = front->pixels + y * front->width + x;
            int by = front->y + y;
            int bx = front->x + x;
            memcpy(back->pixels + by * back->width + bx, pixel, 4);
        }
    }
}
