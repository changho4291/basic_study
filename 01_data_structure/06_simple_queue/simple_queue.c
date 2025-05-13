#include "simple_queue.h"

void queue_init(IntQueue* q) {
    q->front = q->rear = q->size = 0;
}

bool queue_is_empty(IntQueue* q) {
    return q->size == 0;
}

bool queue_is_full(IntQueue* q) {
    return q->size == QUEUE_SIZE;
}

bool queue_enqueue(IntQueue* q, int value) {
    if (queue_is_full(q)) { return false; }
    q->data[q->rear] = value;
    q->rear = (q->rear + 1) % QUEUE_SIZE;
    q->size++;
    return true;
}

bool queue_dequeue(IntQueue* q, int* out) {
    if (queue_is_empty(q)) { return false; }
    *out = q->data[q->front];
    q->front = (q->front + 1) % QUEUE_SIZE;
    q->size--;
}