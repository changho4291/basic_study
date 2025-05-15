#include "doubly_linked_list.h"
#include <stdio.h>

int main() {
    DoublyLinkedList list;
    dlist_init(&list);

    dlist_push_back(&list, 10);
    dlist_push_back(&list, 20);
    dlist_push_back(&list, 30);
    dlist_print_forward(&list);    // 10 <-> 20 <-> 30 <-> NULL

    dlist_push_front(&list, 5);
    dlist_print_forward(&list);    // 5 <-> 10 <-> 20 <-> 30 <-> NULL

    DNode* node = dlist_find(&list, 10);    // 10 노드
    dlist_insert_after(node, 15);   // 10 뒤에 15 삽입
    dlist_print_forward(&list);    // 5 <-> 10 <-> 15 <-> 20 <-> 30 <-> NULL

    dlist_remove(&list, 20);
    dlist_print_forward(&list);    // 5 <-> 10 <-> 15 <-> 30 <-> NULL

    dlist_print_revers(&list);      // 30 <-> 15 <-> 10 <-> 5 <-> NULL

    dlist_clear(&list);
    dlist_print_forward(&list);    // NULL

    return 0;
}