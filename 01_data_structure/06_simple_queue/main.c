#include "simple_queue.h"

int main(int argc, char const *argv[]) {
    IntQueue q;
    queue_init(&q);

    for (int i = 1; i <= 5; i++) {
        queue_enqueue(&q, i * 10);
    }

    int val;
    while (queue_dequeue(&q, &val)) {
        printf("Dequeued: %d\n", val);
    }
    
    return 0;
}
