#include "simple_stack.h"

void init(Stack* s) {
    s->top = -1;
}

bool is_empty(Stack* s) {
    return s->top == -1;
}

bool is_full(Stack* s) {
    return s->top == MAX_SIZE - 1;
}

bool push(Stack* s, int value) {
    if (is_full(s)) {
        printf("Stack overflow\n");
        return false;
    }
    s->data[++(s->top)] = value;
    return true;
}

bool pop(Stack* s, int* out) {
    if (is_empty(s)) {
        printf("Stack underflow\n");
        return false;
    }
    *out = s->data[(s->top)--];
    return true;
}

bool peek(Stack* s, int* out) {
    if (is_empty(s)) {
        return false;
    }
    *out = s->data[s->top];
    return true;
}

