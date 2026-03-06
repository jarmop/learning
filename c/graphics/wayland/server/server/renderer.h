#ifndef RENDERER_H
#define RENDERER_H

#include <stdint.h>

void rd_paint(struct pxgrid *pxgrid, uint32_t color);

void rd_blend(struct pxgrid *back, struct pxgrid *front);

#endif