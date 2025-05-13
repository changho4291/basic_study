#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct {
    void* buffer;
    size_t elem_size;
    size_t capacity;
    size_t front, rear, size;
} GenericQueue;

int generic_queue_init(GenericQueue* q, size_t elem_size, size_t capacity);

void generic_queue_free(GenericQueue* q);

bool generic_queue_enqueue(GenericQueue* q, const void* elem);

bool generic_queue_dequeue(GenericQueue* q, void* out);