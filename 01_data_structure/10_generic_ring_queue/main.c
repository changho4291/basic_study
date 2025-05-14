#include "generic_queue.h"
#include <stdint.h>
#include <stdio.h>

#define POOL_RING_BLOCK_SIZE (sizeof(int) * 5)   // 원형큐 버퍼 크기
#define POOL_BLOCK_SIZE POOL_RING_BLOCK_SIZE     // 풀 블록 크기
#define POOL_BLOCK_COUNT 1                       // 1개 블록만 할당 (테스트)

int main(int argc, char const *argv[]) {
    uint8_t pool_buffer[POOL_BLOCK_SIZE * POOL_BLOCK_COUNT];
    MemoryPool dummy_pool = {0};    // 테스트용 더미 pool

    memory_pool_init(&dummy_pool, pool_buffer, POOL_BLOCK_SIZE, POOL_BLOCK_COUNT);

    PoolRingQueue q;
    pool_ring_queue_init(&q, &dummy_pool, sizeof(int), 5);

    int values[] = {11, 22, 33};
    for (int i = 0; i < 3; i++) {
        pool_ring_queue_enqueue(&q, &values[i]);
    }

    int out;
    while (pool_ring_queue_dequeue(&q, &out)) {
        printf("Dequeued: %d\n", out);
    }
    
    return 0;
}
