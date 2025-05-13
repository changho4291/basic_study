#ifndef __GENERIC_QUEUE_H__
#define __GENERIC_QUEUE_H__

#include "memory_pool.h"
#include <string.h>
#include <stdbool.h>

typedef struct {
    void* buffer;
    size_t elem_size;
    size_t capacity;
    size_t head, tail, size;
    MemoryPool* pool;
} PoolRingQueue;

int pool_ring_queue_init(PoolRingQueue* q, MemoryPool* pool
    , size_t elem_size, size_t capacity);

bool pool_ring_queue_enqueue(PoolRingQueue* q, const void* elem);

bool pool_ring_queue_dequeue(PoolRingQueue* q, void* out);

#endif // __GENERIC_QUEUE_H__