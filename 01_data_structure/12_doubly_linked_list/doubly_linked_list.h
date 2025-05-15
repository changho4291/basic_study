#ifndef __DOUBLY_LINKED_LIST_H__
#define __DOUBLY_LINKED_LIST_H__

#include <stdbool.h>

typedef struct DNode {
    int data;
    struct DNode* prev;
    struct DNode* next;
} DNode;

typedef struct {
    DNode* head;
    DNode* tail;
} DoublyLinkedList;

// 초기화
void dlist_init(DoublyLinkedList* list);

// 앞에 삽입
void dlist_push_front(DoublyLinkedList* list, int value);

// 뒤에 삽입
void dlist_push_back(DoublyLinkedList* list, int value);

// 특정 노드 뒤에 삽입
bool dlist_insert_after(DNode* node, int value);

// 값으로 노드 찾기
DNode* dlist_find(const DoublyLinkedList* list, int value);

// 값으로 노드 삭제
bool dlist_remove(DoublyLinkedList* list, int value);

// 출력 (앞->뒤)
void dlist_print_forward(const DoublyLinkedList* list);

// 출력 (뒤->앞)
void dlist_print_revers(const DoublyLinkedList* list);

// 리스트 삭제
void dlist_clear(DoublyLinkedList* list);

#endif // __DOUBLY_LINKED_LIST_H__