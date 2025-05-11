#include "generic_stack.h"
#include <string.h>

GenericStack * stack_create(MemoryPool* pool, size_t elem_size, size_t capacity) {
    GenericStack* s = (GenericStack*)memory_pool_alloc(pool);
    if (!s) { return NULL; }

    s->data = memory_pool_alloc(pool);
    if (!s->data) {
        memory_pool_free(pool, s);
        return NULL;
    }

    s->elem_size = elem_size;
    s->capacity = capacity;
    s->top = 0;
    return s;
}

void stack_destroy(MemoryPool* pool, GenericStack* stack) {
    if (stack) {
        memory_pool_free(pool, stack->data);
        memory_pool_free(pool, stack);
    }
}

int stack_push(GenericStack* stack, const void* elem) {
    if (stack->top >= stack->capacity) return 0;
    void* target = (char*)stack->data + stack->top * stack->elem_size;
    memcpy(target, elem, stack->elem_size);
    stack->top++;
    return 1;
}

int stack_pop(GenericStack* stack, void* out) {
    if (stack->top == 0) { return 0; }
    stack->top--;
    void* source = (char*)stack->data + stack->top * stack->elem_size;
    if (out) { memcpy(out, source, stack->elem_size); }
    return 1;
}

int stack_peek(GenericStack* stack, void* out) {
    if (stack->top == 0) { return 0; }
    void* source = (char*)stack->data + (stack->top - 1) * stack->elem_size;
    if (out) { memcpy(out, source, stack->elem_size); }
    return 1;
}

int stack_is_empty(const GenericStack* stack) {
    return stack->top == 0;
}