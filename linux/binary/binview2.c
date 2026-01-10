#include <stdio.h>
#include <stdlib.h>

// #define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Give filename\n");
        return 0;
    }
    char *filename = argv[1];
    printf("Reading file: %s\n", filename);

    FILE *fp;
    size_t ret;
    unsigned int buffer_length = 4;
    unsigned char buffer[buffer_length];
    unsigned int buffer_value_size = sizeof(*buffer);

    fp = fopen(filename, "rb");
    if (!fp) {
        perror("fopen");
        return EXIT_FAILURE;
    }

    ret = fread(buffer, buffer_value_size, buffer_length, fp);
    if (ret != buffer_length) {
        fprintf(stderr, "fread() failed: %zu\n", ret);
        exit(EXIT_FAILURE);
    }

    printf("ELF magic: %x, %x, %x, %x\n", buffer[0], buffer[1],
            buffer[2], buffer[3]);

    ret = fread(buffer, 1, 1, fp);
    if (ret != 1) {
        fprintf(stderr, "fread() failed: %zu\n", ret);
        exit(EXIT_FAILURE);
    }

    printf("Class: %x\n", buffer[0]);

    fclose(fp);

    exit(EXIT_SUCCESS);
}