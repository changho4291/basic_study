#include "generic_stack.h"
#include <stdio.h>

void print_int(const void* data) {
    printf("%d\n", *(const int*)data);
}

int main() {
    Stack* s = stack_create(sizeof(int), NULL);

    for (int i = 1; i <= 5; ++i) {
        stack_push(s, &i);
    }

    printf("스택 내용\n");
    stack_print(s, print_int);

    int x;
    while (stack_pop(s, &x)) {
        printf("Pop: %d\n", x);
    }
    
    stack_destroy(s);
    return 0;
}