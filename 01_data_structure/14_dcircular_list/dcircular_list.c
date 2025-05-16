#include "dcircular_list.h"
#include <stdlib.h>
#include <stdio.h>

void dclist_init(DCircularList* list) {
    list->head = (DNode*)malloc(sizeof(DNode));
    list->head->next = list->head;
    list->head->prev = list->head;
}

void dclist_clear(DCircularList* list) {
    DNode* curr = list->head->next;
    while (curr != list->head) {
        DNode* next = curr->next;
        free(curr);
        curr = next;
    }
    free(list->head);
    list->head = NULL;
}

static void insert_between(DNode* prev, DNode* next, DNode* node) {
    prev->next = node;
    node->prev = prev;
    node->next = next;
    next->prev = node;
}

void dclist_push_front(DCircularList* list, int value) {
    DNode* node = (DNode*)malloc(sizeof(DNode));
    node->data = value;
    insert_between(list->head, list->head->next, node);
}

void dclist_push_back(DCircularList* list, int value) {
    DNode* node = (DNode*)malloc(sizeof(DNode));
    node->data = value;
    insert_between(list->head->prev, list->head, node);
}

bool dclist_insert_after(DNode* node, int value) {
    if (!node) { return false; }
    DNode* new_node = (DNode*)malloc(sizeof(DNode));
    new_node->data = value;
    insert_between(node, node->next, new_node);
    return true;
}

bool dclist_remove_value(DCircularList* list, int value) {
    DNode* curr = list->head->next;
    while (curr != list->head) {
        if (curr->data = value) {
            curr->prev->next = curr->next;
            curr->next->prev = curr->prev;
            free(curr);
            return true;
        }
        curr = curr->next;
    }
    return false;
}

DNode* dclist_find(const DCircularList* list, int value) {
    DNode* curr = list->head->next;
    while (curr != list->head) {
        if (curr->data == value) { return curr; }
        curr = curr->next;
    }
    return NULL;
}

void dclist_print_forward(const DCircularList* list) {
    DNode* curr = list->head->next;
    while (curr != list->head) {
        printf("%d <-> ", curr->data);
        curr = curr->next;
    }
    printf("(head)\n");
}

void dclist_print_reverse(const DCircularList* list) {
    DNode* curr = list->head->prev;
    while (curr != list->head) {
        printf("%d <-> ", curr->data);
        curr = curr->prev;
    }
    printf("(head)\n");
}