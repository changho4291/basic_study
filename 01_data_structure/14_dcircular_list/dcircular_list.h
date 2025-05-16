#ifndef __DCIRCULAR_LIST_H__
#define __DCIRCULAR_LIST_H__

#include <stdbool.h>

typedef struct DNode {
    int data;
    struct DNode* prev;
    struct DNode* next;
} DNode;

typedef struct {
    DNode* head;    // dummy head node (head->next = first, head->prev = list
} DCircularList;

// 초기화 및 정리
void dclist_init(DCircularList* list);
void dclist_clear(DCircularList* list);

// 삽입 
void dclist_push_front(DCircularList* list, int value);
void dclist_push_back(DCircularList* list, int value);
bool dclist_insert_after(DNode* node, int value);

// 삭제
bool dclist_remove_value(DCircularList* list, int value);

// 검색
DNode* dclist_find(const DCircularList* list, int value);

// 출력 
void dclist_print_forward(const DCircularList* list);
void dclist_print_reverse(const DCircularList* list);

#endif // __DCIRCULAR_LIST_H__