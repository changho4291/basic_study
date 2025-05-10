#include "generic_stack.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define INITAL_CAPACITY 8

struct Stack {
    void* data;         // 원소를 저장하는 메모리 블록 (동적 배열)
    size_t elem_size;   // 원소 하나의 크기 (바이트 단위)
    int top;            // 스택의 현재 상위 인덱스 (비어있으면 -1)
    int capacity;       // data 배열의 최대 원소 개수 (현재 할당 크기)
    FreeFunc free_fn;   // 사용자 정의 원소 메모리 해제 함수 (NULL 이면 사용 안함)
};

Stack* stack_create(size_t elem_size, FreeFunc free_fn) {
    Stack* s = malloc(sizeof(Stack));
    s->elem_size = elem_size;
    s->capacity = INITAL_CAPACITY;
    s->top = -1;
    s->data = malloc(s->capacity * elem_size);
    s->free_fn = free_fn;
    return s;
}

void stack_destroy(Stack* s) {
    if (s->free_fn) {
        for (int i = 0; i < s->top; ++i) {
            void* elem = (char*)s->data + i * s -> elem_size;
            s->free_fn(elem);
        }
    }
    free(s->data);
    free(s);
}

static int stack_resize(Stack* s) {
    s->capacity *= 2;
    void* new_data = realloc(s->data, s->elem_size * s->capacity);
    if (!new_data) return 0;
    s->data = new_data;
    return 1;
}

int stack_push(Stack* s, const void* elem) {
    if (s->top + 1 >= s->capacity) {
        if (!stack_resize(s)) return 0;
    }
    s->top++;
    void* target = (char*)s->data + s->top * s->elem_size;
    memcpy(target, elem, s->elem_size);
    return 1;
}

int stack_pop(Stack* s, void* opt) {
    if (s->top < 0) return 0;
    void* source = (char*)s->data + s->top * s->elem_size;
    memcpy(opt, source, s->elem_size);
    s->top--;
    return 1;
}

int stack_peek(Stack* s, void* out) {
    if (s->top < 0) return 0;
    void* source = (char*)s->data + s->top * s->elem_size;
    memcpy(out, source, s->elem_size);
    return 1;
}

int stack_is_empty(const Stack* s) {
    return s->top < 0;
}

void stack_print(const Stack* s, PrintFunc print_fn) {
    for (int i = s->top; i > 0; --i) {
        void* elem = (char*)s->data + i * s->elem_size;
        print_fn(elem);
    }
}