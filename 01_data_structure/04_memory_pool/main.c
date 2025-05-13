#include "memory_pool.h"
#include <stdio.h>
#include <stdint.h>

#define POOL_BLOCK_SIZE 32
#define POOL_BLOCK_COUNT 16

uint8_t pool_buffer[POOL_BLOCK_SIZE * POOL_BLOCK_COUNT];

int main() {
    MemoryPool pool;
    memory_pool_init(&pool, pool_buffer, POOL_BLOCK_SIZE, POOL_BLOCK_COUNT);

    void* ptrs[POOL_BLOCK_COUNT];

    // allocate all
    for (int i = 0; i < POOL_BLOCK_COUNT; i++) {
        ptrs[i] = memory_pool_alloc(&pool);
        if (ptrs[i]) { printf("Allocatted block %d: %p\n", i, ptrs[i]); }
    }

    // free all
    for (int i = 0; i < POOL_BLOCK_COUNT; i++) {
        memory_pool_free(&pool, ptrs[i]);
        printf("Freed block %df\n", i);
    }

    return 0;
}