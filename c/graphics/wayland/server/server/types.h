#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>

struct pxgrid {
    uint32_t *pixels;
    uint16_t width;
    uint16_t height;
    int16_t x;
    int16_t y;
};

#endif