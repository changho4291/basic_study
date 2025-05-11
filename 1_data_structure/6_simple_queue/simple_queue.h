#ifndef __SIMPLE_QUEUE_H__
#define __SIMPLE_QUEUE_H__

#include <stdio.h>
#include <stdbool.h>

#define QUEUE_SIZE 10

typedef struct IntQueue {
    int data[QUEUE_SIZE];
    int front, rear, size;
} IntQueue;

void queue_init(IntQueue* q);

bool queue_is_empty(IntQueue* q);

bool queue_is_full(IntQueue* q);

bool queue_enqueue(IntQueue* q, int value);

bool queue_dequeue(IntQueue* q, int* out);

#endif // __SIMPLE_QUEUE_H__