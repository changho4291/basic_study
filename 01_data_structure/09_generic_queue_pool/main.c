#include "generic_queue.h"

#define POOL_BLOCK_SIZE (sizeof(QueueNode) + sizeof(int))
#define POOL_BLOCK_COUNT 10

int main(int argc, char const *argv[]) {
    uint8_t pool_buffer[POOL_BLOCK_SIZE * POOL_BLOCK_COUNT];
    MemoryPool pool;
    memory_pool_init(&pool, pool_buffer, POOL_BLOCK_SIZE, POOL_BLOCK_COUNT);

    PoolQueue q;
    pool_queue_init(&q, &pool, sizeof(int));

    int values[] = {5, 10, 15};
    for (int i = 0; i < 3; i++) {
        pool_queue_enqueue(&q, &values[i]);
    }

    int out;
    while (pool_queue_dequeue(&q, &out)) {
        printf("Dequeued: %d\n", out);
    }

    return 0;
}
