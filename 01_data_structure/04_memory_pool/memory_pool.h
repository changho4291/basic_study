#ifndef __MEMORY_POOL_H__
#define __MEMORY_POOL_H__

#include <stddef.h>

typedef struct MemoryPool {
    void* free_list;
    size_t block_size;
    size_t block_count;
    void* pool_start;
} MemoryPool;

int memory_pool_init(MemoryPool* pool, void* buffer, size_t block_size, size_t block_count);
void* memory_pool_alloc(MemoryPool* pool);
void memory_pool_free(MemoryPool* pool, void* block);

#endif // __MEMORY_POOL_H__