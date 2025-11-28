#include <unistd.h>
#include <stdlib.h>

#define BUF_SIZE 100

/**
 * For testing how process memory looks like in /proc/PID/maps
 */
int main() {
    // int *foo = malloc(1024);
    int foo[] = {};
    char buf[BUF_SIZE];
    ssize_t numRead;
    while ((numRead = read(STDIN_FILENO, buf, BUF_SIZE)) > 0)  {
        foo[numRead] = numRead;
        write(STDOUT_FILENO, buf, numRead);

    }

    return 1;
}