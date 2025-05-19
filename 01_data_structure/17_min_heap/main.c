#include "heap.h"
#include <stdio.h>

int main(int argc, char const *argv[]) {
    MinHeap* heap = heap_create(20);

    int values[] = {50, 30, 40, 10, 5, 70};
    for (int i = 0; i < 6; i++) {
        heap_insert(heap, values[i]);
    }

    printf("힙 상태: ");
    heap_print(heap);

    int min;
    heap_remove_min(heap, &min);
    printf("최소값 %d 제거 후: ", min);
    heap_print(heap);

    heap_insert(heap, 1);
    heap_insert(heap, 2);
    printf("힙 상태: ");
    heap_print(heap);

    heap_free(heap);
    return 0;
}
