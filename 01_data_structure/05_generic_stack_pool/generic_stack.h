#ifndef __GENERIC_STACK_H__
#define __GENERIC_STACK_H__

#include "memory_pool.h"
#include <stddef.h>

typedef struct GenericStack {
    void* data;
    size_t elem_size;
    size_t capacity;
    size_t top;
} GenericStack;

GenericStack * stack_create(MemoryPool* pool, size_t elem_size, size_t capacity);
void stack_destroy(MemoryPool* pool, GenericStack* stack);

int stack_push(GenericStack* stack, const void* elem);
int stack_pop(GenericStack* stack, void* out);
int stack_peek(GenericStack* stack, void* out);
int stack_is_empty(const GenericStack* stack);

#endif // __GENERIC_STACK_H__