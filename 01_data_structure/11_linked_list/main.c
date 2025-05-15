#include "linked_list.h"

int main(int argc, char const *argv[]) {
    LinkedList list;
    list_init(&list);

    list_push_back(&list, 10);
    list_push_back(&list, 20);
    list_push_back(&list, 30);
    list_print(&list);  // 10 -> 20 -> 30 -> NULL

    list_push_front(&list, 5);
    list_print(&list);  // 5 -> 10 - > 20 -> 30 -> NULL

    list_remove(&list, 20);
    list_print(&list);  // 5 -> 10 -> 30 -> NULL;

    Node* target = list_find_node(&list, 10);   // 10 뒤에
    list_insert_after(target, 25);              // insert 25 after 10
    list_print(&list);  // 5 -> 10 -> 25 -> 30 -> NULL

    list_clear(&list);
    list_print(&list);  // NULL

    return 0;
}
