#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>

struct pxgrid {
    uint32_t *pixels;
    uint16_t width;
    uint16_t height;
    uint16_t x;
    uint16_t y;
};

#endif