#include <stdint.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

void drawGradient(uint16_t width, uint16_t height, uint8_t *map, uint32_t pitch);
void drawCheckers(uint16_t width, uint16_t height, uint8_t *map, uint32_t pitch);
void drawPlain(uint16_t width, uint16_t height, uint8_t *map, uint32_t pitch);
void drawMouse(uint16_t mouse_x, uint16_t mouse_y, uint8_t *map, uint32_t pitch);
void mouse();