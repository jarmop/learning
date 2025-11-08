#include <stdlib.h>
#include <stdio.h>

int main() {
    char foo[2];    // A pointer to the beginning of a memory block reserved for two characters
    *(foo+1) = 'a'; // A messy way of setting a value for the second character
    foo[1] = 'a';   // The same as above but with a nicer syntax


    printf("Hello, %c!\n", *(foo+1));

    return EXIT_SUCCESS;
}