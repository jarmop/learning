#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>

int main(void) {
    int fb = open("/dev/fb0", O_RDWR);
    if (fb < 0) {
        perror("open");
        return 1;
    }

    int width = 1280;
    int height = 800;
    int bytes_per_pixel = 4;
    int stride = width * bytes_per_pixel;
    size_t size = stride * height;

    uint32_t *fbmem = mmap(NULL, size,
                           PROT_READ | PROT_WRITE,
                           MAP_SHARED, fb, 0);

    if (fbmem == MAP_FAILED) {
        perror("mmap");
        close(fb);
        return 1;
    }

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            uint8_t r = (x * 255) / width;
            uint8_t g = (y * 255) / height;
            uint8_t b = 128;
            fbmem[y * width + x] =
                (r << 16) | (g << 8) | b;
        }
    }

    sleep(5);

    munmap(fbmem, size);
    close(fb);
    return 0;
}
