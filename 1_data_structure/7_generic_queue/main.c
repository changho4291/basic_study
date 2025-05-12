#include "generic_queue.h"
#include <stdio.h>

int main(int argc, char const *argv[]) {
    GenericQueue q;
    if(!generic_queue_init(&q, sizeof(float), 5)) { return -1; }

    float values[] = {1.1f, 2.2f, 3.3f};
    for(int i = 0; i < 3; i++) {
        generic_queue_enqueue(&q, &values[i]);
    }

    float out;
    while (generic_queue_dequeue(&q, &out)) {
        printf("Dequeued: %.2f\n", out);
    }
    
    generic_queue_free(&q);
    return 0;
}
