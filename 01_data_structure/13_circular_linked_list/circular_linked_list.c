#include "circular_linked_list.h"
#include <stdio.h>
#include <stdlib.h>

void clist_init(CircularList* list) {
    list->tail = NULL;
}

void clist_push_front(CircularList* list, int value) {
    CNode* node = (CNode*)malloc(sizeof(CNode));
    node->data = value;

    if (!list->tail) {
        node->next = node;
        list->tail = node;
    } else {
        node->next = list->tail->next;
        list->tail->next = node;
    }
}

void clist_push_back(CircularList* list, int value) {
    clist_push_front(list, value);
    list->tail = list->tail->next;  // 새로 추가된 노드를 tail로 변경
}

bool clist_remove(CircularList* list, int value) {
    if (!list->tail) { return false; }

    CNode* prev = list->tail;
    CNode* curr = list->tail->next;

    do {
        if (curr->data == value) {
            if (curr == prev) { // 리스트에 1개 노드만 존재
                list->tail = NULL;
            } else {
                prev->next = curr->next;
                if (curr == list->tail) { list->tail = prev; }
            }
            free(curr);
            return true;
        }
        prev = curr;
        curr = curr->next;
    } while (curr != list->tail->next);
    
    return false;
}

CNode* clist_find(const CircularList* list, int value) {
    if (!list->tail) { return NULL; }

    CNode* curr = list->tail->next;
    do {
        if (curr->data == value) { return curr; }
        curr = curr->next;
    } while (curr != list->tail->next);
    
    return NULL;
}

bool clist_insert_after(CircularList* list, CNode* node, int value) {
    if (!node) { return false; }

    CNode* newnode = (CNode*)malloc(sizeof(CNode));
    if (!newnode) { return false; }

    newnode->data = value;
    newnode->next = node->next;
    node->next = newnode;

    // 만약 node가 tail이면 새로 넣은 노드를 tail로 변경
    if (node == list->tail) { list->tail = newnode; }

    return true;
}

void clist_print(const CircularList* list) {
    if (!list->tail) {
        printf("List is emptu.\n");
        return;
    }

    CNode* curr = list->tail->next;
    do {
        printf("%d -> ", curr->data);
        curr = curr->next;
    } while (curr != list->tail->next);
    printf("(back to head)\n");
}

void clist_clear(CircularList* list) {
    if (!list->tail) { return; }

    CNode* curr = list->tail->next;
    CNode* start = curr;
    do {
        CNode* next = curr->next;
        free(curr);
        curr = next;
    } while (curr != start);
    
    list->tail = NULL;
}