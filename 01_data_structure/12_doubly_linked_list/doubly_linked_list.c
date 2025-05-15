#include "doubly_linked_list.h"
#include <stdio.h>
#include <stdlib.h>

void dlist_init(DoublyLinkedList* list) {
    list->head = list->tail = NULL;
}

void dlist_push_front(DoublyLinkedList* list, int value) {
    DNode* node = (DNode*)malloc(sizeof(DNode));
    node->data = value;
    node->prev = NULL;
    node->next = list->head;

    if (list->head) {
        list->head->prev = node;
    } else {
        list->tail = node;  // 첫 노드 tail도 설정
    }

    list->head = node;
}

void dlist_push_back(DoublyLinkedList* list, int value) {
    DNode* node = (DNode*)malloc(sizeof(DNode));
    node->data = value;
    node->next = NULL;
    node->prev = list->tail;

    if (list->tail) {
        list->tail->next = node;
    } else {
        list->head = node;
    }

    list->tail = node;
}

bool dlist_insert_after(DNode* node, int value) {
    if (!node) { return false; }

    DNode* new_node =(DNode*)malloc(sizeof(DNode));
    new_node->data = value;
    new_node->prev = node;
    new_node->next = node->next;

    if (node->next) {
        node->next->prev = new_node;
    }

    node->next = new_node;

    return true;
}

DNode* dlist_find(const DoublyLinkedList* list, int value) {
    DNode* current = list->head;
    while (current) {
        if (current->data == value) { return current; }
        current = current->next;
    }
    return NULL;    // 찾는 값이 없음
}

bool dlist_remove(DoublyLinkedList* list, int value) {
    DNode* current = list->head;
    while (current) {
        if (current->data == value) {
            if (current->prev) { current->prev->next = current->next; }
            else { list->head = current->next; }

            if (current->next) { current->next->prev = current->prev; }
            else { list->tail = current->prev; }

            free(current);
            return true;
        }
        current = current->next;
    }
    return false;
}

void dlist_print_forward(const DoublyLinkedList* list) {
    DNode* current = list->head;
    while (current) {
        printf("%d <-> ", current->data);
        current = current->next;
    }
    printf("NULL\n");
}

void dlist_print_revers(const DoublyLinkedList* list) {
    DNode* current = list->tail;
    while (current) {
        printf("%d <-> ", current->data);
        current = current->prev;
    }
    printf("NULL\n");
}

void dlist_clear(DoublyLinkedList* list) {
    DNode* current = list->head;
    while (current) {
        DNode* next = current->next;
        free(current);
        current = next;
    }
    list->head = list->tail = NULL;
}