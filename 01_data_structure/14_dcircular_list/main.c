#include "dcircular_list.h"
#include <stdio.h>

int main(int argc, char const *argv[]) {
    DCircularList list;
    dclist_init(&list);

    dclist_push_back(&list, 10);
    dclist_push_back(&list, 20);
    dclist_push_back(&list, 30);
    dclist_print_forward(&list);    // 10 <-> 20 <-> 30 <-> head

    dclist_push_front(&list, 5);
    dclist_print_forward(&list);    // 5 <-> 10 <-> 20 <-> 30 <-> head

    DNode* node = dclist_find(&list, 20);
    if (node) { dclist_insert_after(node, 25); }
    dclist_print_forward(&list);    // 5 <-> 10 <-> 20 <-> 25 <-> 30 <-> head

    printf("Reverse: \n");
    dclist_print_reverse(&list);    // 30 <-> 25 <-> 20 <-> 10 <-> 5 <-> head

    dclist_clear(&list);
    return 0;
}
