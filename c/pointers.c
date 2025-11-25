#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

void array_access() {
    char foo[2];    // A pointer to the beginning of a memory block reserved for two characters
    *(foo+1) = 'a'; // A messy way of setting a value for the second character
    foo[1] = 'a';   // The same as above but with a nicer syntax

    // Print the second character
    printf("%c\n", *(foo+1));

    // Print the address of the first character in hexadecimal
    // printf("%p\n", foo);

    // Print the address of the first character in decimal
    printf("%ld\n", (uintptr_t) foo);

    /**
     * Integer uses four bytes, and each byte has its own address, so accessing
     * the second value in an array requires incrementing the pointer by four.
     * But this happens automatically: "int_arr+1" actually increments by four.
     */
    int int_arr[] = {1,2};
    printf("%d\n", int_arr[1]);
    printf("%d\n", *(int_arr+1));

    printf("%ld\n", (uintptr_t) int_arr);
    printf("%ld\n", (uintptr_t) (int_arr+1));
}

void array_addresses() {
    char char_array1[1];
    char char_array2[1];
    int int_array1[1];
    int int_array2[1];
    long long_array1[1];
    long long_array2[1];
    float float_array1[1];
    float float_array2[1];

    /**
     * Number arrays seem to be stored before character arrays (in lower
     * memory addresses). There are two extra bits separating the number
     * arrays from the character arrays. Arrays are ordered by size within 
     * their type groups, so that smaller char arrays are stored before the 
     * bigger char arrays. If floating point and integer are the same size 
     * (e.g. float and int), the integer array is stored first
     */
    printf("char arrays:\n");
    printf("%ld\n", (uintptr_t) char_array1);
    printf("%ld\n", (uintptr_t) char_array2);
    printf("int arrays:\n");
    printf("%ld\n", (uintptr_t) int_array1);
    printf("%ld\n", (uintptr_t) int_array2);
    printf("long arrays:\n");
    printf("%ld\n", (uintptr_t) long_array1);
    printf("%ld\n", (uintptr_t) long_array2);
    printf("float arrays:\n");
    printf("%ld\n", (uintptr_t) float_array1);
    printf("%ld\n", (uintptr_t) float_array2);
}

/**
 * A pointer is just an address. An array is just an address (to the first value
 * in a sequence of values that is stored in adjacent memory slots).
 */
void stack_hacking() {
    int a = 17;
    int b = 5;

    /**
     * Values of a and b are stored in the stack in their declaration order. As
     * they are in adjacent memory slots we can access the value b by getting 
     * the address number of the value a (using the & sign), incrementing that by 1, and then 
     * and getting the value from that address (using the * sign)
     */
    int *address_of_a = &a;
    int *address_of_b = address_of_a + 1;
    int value_of_b = *address_of_b;

    printf("%d\n", value_of_b);

    // In one expression
    printf("%d\n", *(&a+1));

    // Using the array syntax
    printf("%d\n", (&a)[1]);
}

int main() {
    // array_access();
    // array_addresses();

    stack_hacking();

    return EXIT_SUCCESS;
}