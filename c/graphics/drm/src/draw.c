#include <string.h>
#include "drm-demo.h"

void drawGradient(uint16_t width, uint16_t height, uint8_t *map, uint32_t pitch) {
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            uint8_t r = (x * 255) / width;
            uint8_t g = (y * 255) / height;
            uint8_t b = 128;

            // Combine rgb values into one 3 byte value by shifting r bitwise to the 
            // left by 2 bytes, and g by 1 byte, and then doing bitwise or for each
            uint32_t pixel = (r << 16) | (g <<  8) | b;

            memcpy(map + y * pitch + x * 4, &pixel, 4);
        }
    }
}

void drawCheckers(uint16_t width, uint16_t height, uint8_t *map, uint32_t pitch) {
    int yinc = height / 10;
    int xinc = width / 10;
    for (int y = 0; y < height; y ++) {
        int even_y = y/yinc%2 == 0;
        int scale_a, scale_b;
        if (even_y) {
            scale_a = 255; scale_b = 0;
        } else {
            scale_a = 0; scale_b = 255;
        }
        // int scale_a = y/yinc%2 * 255;
        for (int x = 0; x < width; x ++) {
            int even_x = x/xinc%2 == 0;
            int scale;
            if (even_x) {
                scale = scale_a;
            } else {
                scale = scale_b;
            }
            uint8_t r = scale;
            uint8_t g = scale;
            uint8_t b = scale;

            // Combine rgb values into one 3 byte value by shifting r bitwise to the 
            // left by 2 bytes, and g by 1 byte, and then doing bitwise or for each
            uint32_t pixel = (r << 16) | (g <<  8) | b;

            memcpy(map + y * pitch + x * 4, &pixel, 4);
        }
    }
}

void drawPlain(uint16_t width, uint16_t height, uint8_t *map, uint32_t pitch) {
    uint8_t r = 0; uint8_t g = 0; uint8_t b = 100;
    // Combine rgb values into one 3 byte value by shifting r bitwise to the 
    // left by 2 bytes, and g by 1 byte, and then doing bitwise or for each
    uint32_t pixel = (r << 16) | (g <<  8) | b;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            memcpy(map + y * pitch + x * 4, &pixel, 4);
        }
    }
}

void drawMouse(uint16_t mouse_x, uint16_t mouse_y, uint8_t *map, uint32_t pitch) {
    uint16_t mouse_width = 10; uint16_t mouse_height = 10;
    uint8_t r = 255; uint8_t g = 255; uint8_t b = 255;
    // Combine rgb values into one 3 byte value by shifting r bitwise to the 
    // left by 2 bytes, and g by 1 byte, and then doing bitwise or for each
    uint32_t pixel = (r << 16) | (g <<  8) | b;
    for (int y = mouse_y; y < mouse_y + mouse_height; y++) {
        for (int x = mouse_x; x < mouse_x + mouse_width; x++) {
            memcpy(map + y * pitch + x * 4, &pixel, 4);
        }
    }
}