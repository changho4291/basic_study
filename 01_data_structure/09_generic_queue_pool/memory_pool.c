#include "memory_pool.h"
#include <stdint.h>

typedef struct FreeBlock {
    struct FreeBlock* next;
} FreeBlock;

int memory_pool_init(MemoryPool* pool, void* buffer, size_t block_size, size_t block_count) {
    if (!pool || !buffer || block_size < sizeof(void*) || block_count == 0) { return 0; }

    pool->block_size = block_size;
    pool->block_count = block_count;
    pool->pool_start = buffer;
    pool->free_list = NULL;

    // 초기 free list 구성
    uint8_t* p = (uint8_t*)buffer;
    for (size_t i = 0; i < block_count; i++) {
        FreeBlock* block = (FreeBlock*)p;
        block->next = (i == block_count - 1) ? NULL : (FreeBlock*)(p + block_size);
        p += block_size;
    }

    pool->free_list = buffer;
    return 1;
}

void* memory_pool_alloc(MemoryPool* pool) {
    if (!pool->free_list) { return NULL; }

    FreeBlock* block = (FreeBlock*)pool->free_list;
    pool->free_list = block->next;
    return (void*)block;
}

void memory_pool_free(MemoryPool* pool, void* block) {
    if (!block) { return; }

    FreeBlock* free_block = (FreeBlock*)block;
    free_block->next = (FreeBlock*)pool->free_list;
    pool->free_list = free_block;
}