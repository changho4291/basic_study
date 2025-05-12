#include "generic_queue.h"

int generic_queue_init(GenericQueue* q, size_t elem_size, size_t capacity) {
    q->buffer = malloc(elem_size * capacity);
    if (!q->buffer) { return 0; }
    q->elem_size = elem_size;
    q->capacity = capacity;
    q->front = q->rear = q->size = 0;

    return 1;
}

void generic_queue_free(GenericQueue* q) {
    free(q->buffer);
}

bool generic_queue_enqueue(GenericQueue* q, const void* elem) {
    if (q->size == q->capacity) { return false; }
    void* dest = (char*)q->buffer + (q->rear * q->elem_size);
    memcpy(dest, elem, q->elem_size);
    q->rear = (q->rear + 1) % q->capacity;
    q->size++;

    return true;
}

bool generic_queue_dequeue(GenericQueue* q, void* out) {
    if (q->size == 0) { return false; }
    void* src = (char*)q->buffer + (q->front * q->elem_size);
    memcpy(out, src, q->elem_size);
    q->front = (q->front + 1) % q->capacity;
    q->size--;

    return true;
}