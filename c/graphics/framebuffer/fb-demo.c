// fb-demo.c
// Minimal program that writes to /dev/fb0 and draws a color gradient.
// Linux only. Run as root or with appropriate privileges.

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <linux/fb.h>
#include <string.h>
#include <errno.h>

int main(void) {
    const char *fb_path = "/dev/fb0";
    int fb = open(fb_path, O_RDWR);
    if (fb < 0) {
        fprintf(stderr, "failed to open %s: %s\n", fb_path, strerror(errno));
        return 1;
    }

    // Get fixed screen info (line length, mem start)
    struct fb_fix_screeninfo finfo;
    if (ioctl(fb, FBIOGET_FSCREENINFO, &finfo) < 0) {
        fprintf(stderr, "FBIOGET_FSCREENINFO failed: %s\n", strerror(errno));
        close(fb);
        return 1;
    }

    // Get variable screen info (resolution, depth)
    struct fb_var_screeninfo vinfo;
    if (ioctl(fb, FBIOGET_VSCREENINFO, &vinfo) < 0) {
        fprintf(stderr, "FBIOGET_VSCREENINFO failed: %s\n", strerror(errno));
        close(fb);
        return 1;
    }

    printf("Resolution: %ux%u, %u bpp, line_length=%u\n",
           vinfo.xres, vinfo.yres, vinfo.bits_per_pixel, finfo.line_length);

    // Map the framebuffer to memory
    size_t screensize = finfo.smem_len;
    void *fbmem = mmap(NULL, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fb, 0);
    if (fbmem == MAP_FAILED) {
        fprintf(stderr, "mmap failed: %s\n", strerror(errno));
        close(fb);
        return 1;
    }

    // For simplicity assume 32 bits per pixel (ARGB8888) or 24bpp RGB packed.
    if (vinfo.bits_per_pixel != 32 && vinfo.bits_per_pixel != 24) {
        fprintf(stderr, "This demo expects 24 or 32 bpp (found %u). Exiting.\n", vinfo.bits_per_pixel);
        munmap(fbmem, screensize);
        close(fb);
        return 1;
    }

    const unsigned int width = vinfo.xres;
    const unsigned int height = vinfo.yres;
    const unsigned int bpp = vinfo.bits_per_pixel;
    const unsigned int line_len = finfo.line_length; // bytes per scanline

    // Draw: simple color gradient
    for (unsigned int y = 0; y < height; ++y) {
        uint8_t *row = (uint8_t*)fbmem + (size_t)y * line_len;
        for (unsigned int x = 0; x < width; ++x) {
            uint8_t r = (uint8_t)((x * 255) / (width - 1));
            uint8_t g = (uint8_t)((y * 255) / (height - 1));
            uint8_t b = 128;

            if (bpp == 32) {
                // common layout: BGRA or ARGB depending on hardware; use vinfo offsets to be correct
                uint32_t pixel = (r << vinfo.red.offset) | (g << vinfo.green.offset) | (b << vinfo.blue.offset);
                uint32_t *p = (uint32_t*)(row + x * 4);
                *p = pixel;
            } else { // 24 bpp: assume packed RGB
                uint8_t *p = row + x * 3;
                // Common framebuffers are BGR (little-endian). Safer approach is to look at vinfo offsets,
                // but for brevity we write as B,G,R in that order:
                p[0] = b; // blue
                p[1] = g; // green
                p[2] = r; // red
            }
        }
    }

    // Keep the image for 5 seconds
    sleep(5);

    // Clear screen (optional)
    memset(fbmem, 0, screensize);

    munmap(fbmem, screensize);
    close(fb);
    return 0;
}
