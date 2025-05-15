#include "linked_list.h"
#include <stdio.h>
#include <stdlib.h>

void list_init(LinkedList* list) {
    list->head = NULL;
}

Node* list_find_node(LinkedList* list, int value) {
    Node* current = list->head;
    while (current) {
        if (current->data == value) { return current; }
        current = current->next;
    }
    return NULL;
}

void list_push_front(LinkedList* list, int value) {
    Node* node = (Node*)malloc(sizeof(Node));;
    node->data = value;
    node->next = list->head;
    list->head = node;
}

void list_push_back(LinkedList* list, int value) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->data = value;
    node->next = NULL;

    if (!list->head) {
        list->head = node;
        return;
    }

    Node* current = list->head;
    while (current->next) {
        current = current->next;
    }
    current->next = node;
}

bool list_insert_after(Node* node, int value) {
    if (!node) { return false; }

    Node* new_node = (Node*)malloc(sizeof(Node));
    if (!new_node) { return false; }

    new_node->data = value;
    new_node->next = node->next;
    node->next = new_node;

    return true;
}

bool list_remove(LinkedList* list, int value) {
    Node* current = list->head;
    Node* prev = NULL;

    while (current) {
        if (current->data == value) {
            if (prev) {
                prev->next = current->next;
            } else {
                list->head = current->next;
            }
            free(current);
            return true;
        }
        prev = current;
        current = current->next;
    }
    
    return false;
}

void list_print(const LinkedList* list) {
    Node* current = list->head;
    while (current) {
        printf("%d -> ", current->data);
        current = current->next;
    }
    printf("NULL\n");
}

void list_clear(LinkedList* list) {
    Node* current = list->head;
    while (current) {
        Node* next = current->next;
        free(current);
        current = next;
    }
    list->head = NULL;
}