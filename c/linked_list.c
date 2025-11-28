
#include <stdlib.h>
#include <stdio.h>

typedef struct link{
    int value;
    struct link *next; 
} Link;

int main() {
    Link *head = NULL;

    for (int i = 0; i < 5; i++) {
        Link *new_link = (Link*) malloc(sizeof(Link));
        new_link->value = i + 10;
        new_link->next = head;

        head = new_link;

        printf("push %d\n", head->value);
    }

    while (head != NULL) {
        printf("foo %d\n", head->value);
        head = head->next;        
    }

    return 1;
}