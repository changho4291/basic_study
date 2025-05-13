#include "generic_queue.h"

int pool_ring_queue_init(PoolRingQueue* q, MemoryPool* pool
    , size_t elem_size, size_t capacity) {
    q->buffer = memory_pool_alloc(pool);
    if (!q->buffer) { return 0; }
    q->elem_size = elem_size;
    q->capacity = capacity;
    q->head = q->tail = q->size = 0;
    q->pool = pool;

    return 1;
}

bool pool_ring_queue_enqueue(PoolRingQueue* q, const void* elem) {
    if (q->size == q->capacity) { return false; }
    void* dest = (char*)q->buffer + (q->tail * q->elem_size);
    memcpy(dest, elem, q->elem_size);
    q->tail = (q->tail + 1) % q->capacity;
    q->size++;

    return true;
}

bool pool_ring_queue_dequeue(PoolRingQueue* q, void* out) {
    if (q->size == 0) { return false; }
    void* src = (char*)q->buffer + (q->head * q->elem_size);
    memcpy(out, src, q->elem_size);
    q->head = (q->head + 1) % q->capacity;
    q->size--;

    return true;
}