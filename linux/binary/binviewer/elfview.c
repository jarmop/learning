#include <stdio.h>
#include <stdlib.h>

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Give filename\n");
        return 0;
    }
    char *filename = argv[1];
    printf("Reading file: %s\n", filename);

    FILE *fp;
    size_t ret;
    unsigned char buffer[4];

    fp = fopen(filename, "rb");
    if (!fp) {
        perror("fopen");
        return EXIT_FAILURE;
    }

    ret = fread(buffer, sizeof(*buffer), ARRAY_SIZE(buffer), fp);
    if (ret != ARRAY_SIZE(buffer)) {
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