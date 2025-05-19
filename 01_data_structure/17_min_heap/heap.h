#ifndef __HEAP_H__
#define __HEAP_H__

#include <stdbool.h>

typedef struct {
    int* data;
    int capacity;
    int size;
} MinHeap;

// 생성/해제
MinHeap* heap_create(int capacity);
void heap_free(MinHeap* heap);

// 삽입/삭제
bool heap_insert(MinHeap* heap, int value);
bool heap_remove_min(MinHeap* heap, int* out);

// 조회
int heap_get_min(MinHeap* heap);

// 출력
void heap_print(MinHeap* heap);

#endif // __HEAP_H__