#include "generic_queue.h"

void pool_queue_init(PoolQueue* q, MemoryPool* pool, size_t elem_size) {
    q->head = q->tail = NULL;
    q->pool = pool;
    q->elem_size = elem_size;
}

bool pool_queue_enqueue(PoolQueue* q, const void* elem) {
    QueueNode* node = (QueueNode*)memory_pool_alloc(q->pool);
    if (!node) { return false; }
    node->next = NULL;
    memcpy(node->data, elem, q->elem_size);

    if (!q->tail) {
        q->head = q->tail = node;
    } else {
        q->tail->next = node;
        q->tail = node;
    }

    return true;
}

bool pool_queue_dequeue(PoolQueue* q, void* out) {
    if (!q->head) { return false; }
    QueueNode* node = q->head;
    memcpy(out, node->data, q->elem_size);
    q->head = node->next;
    if (!q->head) { q->tail = NULL; }
    memory_pool_free(q->pool, node);

    return true;
}