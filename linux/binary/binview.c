#include <stdio.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Give filename\n");
        return 0;
    }
    char *filename = argv[1];

    unsigned char buffer[10];
    FILE *ptr;

    ptr = fopen(filename,"rb");  // r for read, b for binary

    fread(buffer, sizeof(buffer), 1, ptr); // read 10 bytes to our buffer

    for(int i = 0; i<200; i++)
        printf("%u ", buffer[i]); // prints a series of bytes

    

    return 0;
}