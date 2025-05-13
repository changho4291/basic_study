#ifndef __GENERIC_QUEUE_H__
#define __GENERIC_QUEUE_H__

#include "memory_pool.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

typedef struct QueueNode {
    struct QueueNode* next;
    uint8_t data[];
} QueueNode;

typedef struct {
    QueueNode* head;
    QueueNode* tail;
    MemoryPool* pool;
    size_t elem_size;
} PoolQueue;

void pool_queue_init(PoolQueue* q, MemoryPool* pool, size_t elem_size);

bool pool_queue_enqueue(PoolQueue* q, const void* elem);

bool pool_queue_dequeue(PoolQueue* q, void* out);

#endif // __GENERIC_QUEUE_H__