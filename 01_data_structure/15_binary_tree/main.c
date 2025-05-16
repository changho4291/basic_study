#include "binary_tree.h"
#include <stdio.h>

int main(int argc, char const *argv[]) {
    TreeNode* root = NULL;

    int values[] = {50, 30, 70, 20, 40, 60, 80};
    for (int i = 0; i < sizeof(values)/sizeof(values[0]); i++) {
        root = bt_insert(root, values[i]);
    }

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

    bt_free(root);
    return 0;
}
