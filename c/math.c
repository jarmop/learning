#include <stdio.h>
#include <math.h>

int main() {
    fprintf(stderr, "foo %f\n", sin(0.5));

    return 0;
}