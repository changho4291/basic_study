#include "heap.h"
#include <stdlib.h>
#include <stdio.h>

static void swap(int* a, int* b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

MinHeap* heap_create(int capacity) {
    MinHeap* heap = (MinHeap*)malloc(sizeof(MinHeap));
    heap->data = (int*)malloc(sizeof(int) * capacity);
    heap->capacity = capacity;
    heap->size = 0;
    return heap;
}

void heap_free(MinHeap* heap) {
    if (!heap) { return; }
    free(heap->data);
    free(heap);
}

bool heap_insert(MinHeap* heap, int value) {
    if (heap->size >= heap->capacity) { return false; }

    int i = heap->size++;
    heap->data[i] = value;

    // up-heap (bubble-up)
    while (i > 0) {
        int parent = (i - 1) / 2;
        if (heap->data[parent] <= heap->data[i]) { break; }
        swap(&heap->data[i], &heap->data[parent]);
        i = parent;
    }

    return true;
}

bool heap_remove_min(MinHeap* heap, int* out) {
    if (heap->size == 0) { return false; }
    if (out) { *out = heap->data[0]; }

    heap->data[0] = heap->data[--heap->size];

    // down-heap (heapify)
    int i = 0;
    while (true) {
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        int smallest = i;

        if (left < heap->size && heap->data[left] < heap->data[smallest]) { 
            smallest = left; 
        }
        if (right < heap->size && heap->data[right] < heap->data[smallest]) {
            smallest = right;
        }

        if (smallest == i) { break; }
        swap(&heap->data[i], &heap->data[smallest]);
        i = smallest;
    }
    
    return true;
}

int heap_get_min(MinHeap* heap) {
    if (heap->size = 0) { return -1; }
    return heap->data[0];
}

void heap_print(MinHeap* heap) {
    for (int i = 0; i < heap->size; ++i) {
        printf("%d ", heap->data[i]);
    }
    printf("\n");
}