#include "thread_tree.h"
#include <stdio.h>

int main(int argc, char const *argv[]) {
    TNode* root = NULL;

    int values[] = {50, 30, 70, 20, 40, 60, 80};
    for (int i = 0; i < sizeof(values)/sizeof(values[0]); i++) {
        root = ttree_insert(root, values[i]);
    }

    printf("중위 순회 (스레드 트리): ");
    ttree_inorder(root);    // 20 30 40 50 60 70 80
    printf("\n");

    root = ttree_delete(root, 20);  // leaf
    root = ttree_delete(root, 30);  // one child
    root = ttree_delete(root, 50);  // two children

    printf("삭제 후 중위 순회: ");
    ttree_inorder(root);    // 40 60 70 80
    printf("\n");

    ttree_free(root);
    return 0;
}
