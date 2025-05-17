#include "binary_tree.h"
#include <stdio.h>

int main(int argc, char const *argv[]) {
    TreeNode* root = NULL;

    int values[] = {50, 30, 70, 20, 40, 60, 80};
    for (int i = 0; i < sizeof(values)/sizeof(values[0]); i++) {
        root = bt_insert(root, values[i]);
    }

    printf("전체 노드 수: %d\n", bt_count_nodes(root));
    printf("잎 노드 수: %d\n", bt_count_leaves(root));

    printf("In-order: ");
    bt_inorder(root);
    printf("\n");

    printf("Pre-order: ");
    bt_preorder(root);
    printf("\n");

    printf("Post-order: ");
    bt_postorder(root);
    printf("\n");

    int search_val = 60;
    if (bt_search(root, search_val)) {
        printf("Found %d\n", search_val);
    } else {
        printf("Not found: %d\n", search_val);
    }

    // 삭제 테스트
    root = bt_delete(root, 20); // Case 1: leaf
    root = bt_delete(root, 30); // Case 2: one child
    root = bt_delete(root, 50); // Case 3: two children

    printf("삭제 후 트리 (in-order): ");
    bt_inorder(root);
    printf("\n");

    bt_free(root);
    return 0;
}
