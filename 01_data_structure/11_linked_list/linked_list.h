#ifndef __LINKED_LIST_H__
#define __LINKED_LIST_H__

#include <stdbool.h>

typedef struct Node {
    int data;
    struct Node* next;
} Node;

typedef struct {
    Node* head;
} LinkedList;

// 리스트 초기화
void list_init(LinkedList* list);

// 특정 노드 찾기
Node* list_find_node(LinkedList* list, int value);

// 리스트 앞에 삽입
void list_push_front(LinkedList* list, int value);

// 리스트 뒤에 삽입
void list_push_back(LinkedList* list, int value);

// 특정 노드 뒤에 삽입
bool list_insert_after(Node* node, int value);

// 리스트에서 값 삭제
bool list_remove(LinkedList* list, int value);

// 리스트 출력
void list_print(const LinkedList* list);

// 리스트 메모리 해제
void list_clear(LinkedList* list);

#endif // __LINKED_LIST_H__