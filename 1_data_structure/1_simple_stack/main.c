#include "simple_stack.h"

int main(int argc, char const *argv[]) {
    Stack s;
    init(&s);

    push(&s, 10);
    push(&s, 20);
    push(&s, 30);

    int value;
    while (pop(&s, &value)) {
        printf("Popped: %d\n", value);
    }
    
    return 0;
}
