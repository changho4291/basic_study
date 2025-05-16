#ifndef __CIRCULAR_LINKED_LIST_H__
#define __CIRCULAR_LINKED_LIST_H__

#include <stdbool.h>

typedef struct CNode {
    int data;
    struct CNode* next;
} CNode;

typedef struct {
    CNode* tail;    // tail을 유지 -> tail->next가 head
} CircularList;

// 초기화
void clist_init(CircularList* list);

// 리스트 앞에 삽입 (head 기준)
void clist_push_front(CircularList* list, int value);

// 리스트 뒤에 삽입 (tail 기준)
void clist_push_back(CircularList* list, int value);

// 값으로 노드 삭제
bool clist_remove(CircularList* list, int value);

// 값으로 노드 찾기
CNode* clist_find(const CircularList* list, int value);

// 특정 노드 뒤에 삽입
bool clist_insert_after(CircularList* list, CNode* node, int value);

// 리스트 출력
void clist_print(const CircularList* list);

// 리스트 메모리 해제
void clist_clear(CircularList* list);

#endif // __CIRCULAR_LINKED_LIST_H__