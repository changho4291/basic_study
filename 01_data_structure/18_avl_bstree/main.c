#include "bstree.h"
#include <stdio.h>

int main(int argc, char const *argv[]) {
    Node* root = NULL;
    int values[] = {30, 10, 20, 40, 50, 25};
    for (int i = 0; i < 6; ++i) {
        root = bst_insert(root, values[i]);
    }

    printf("중위 순회: ");
    bst_inorder(root);  // 10 20 25 30 40 50
    printf("\n");

    int find = 25;
    printf("검색 %d: %s\n", find, bst_serch(root,find) ? "O" : "X");

    bst_free(root);
    return 0;
}
