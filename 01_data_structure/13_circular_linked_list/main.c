#include "circular_linked_list.h"
#include <stdio.h>

int main(int argc, char const *argv[]) {
    CircularList list;
    clist_init(&list);

    clist_push_back(&list, 10);
    clist_push_back(&list, 20);
    clist_push_back(&list, 30);
    clist_print(&list); // 10 -> 20 -> 30 -> (back to head)

    clist_push_front(&list, 5);
    clist_print(&list); // 5 -> 10 -> 20 -> 30 -> (back to head)

    CNode* node = clist_find(&list, 20);
    if (node) { printf("Fount node: %d\n", node->data); }

    clist_insert_after(&list, node, 40);
    clist_print(&list); // 5 -> 10  -> 20 -> 40 -> 30 -> (back to head)

    clist_remove(&list, 10);
    clist_print(&list); // 5 -> 20 -> 40 -> 30 -> (back to head)

    clist_clear(&list);
    clist_print(&list); // List is empty

    return 0;
}
