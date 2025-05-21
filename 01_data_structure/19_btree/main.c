#include "btree.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[]) {
    BTree* tree = btree_create();
    int keys[] = {10, 20, 5, 6, 12, 30, 7, 17};

    for (int i = 0; i < sizeof(keys) / sizeof(int); ++i) {
        btree_insert(tree, keys[i]);
    }

    printf("B-트리 중위 순회 결과: ");
    btree_traverse(tree->root);
    printf("\n");

    int k = 6;
    if (btree_search(tree->root, k)) {
        printf("%d 찾음\n", k);
    } else {
        printf("%d 없음\n", k);
    }

    btree_free(tree->root);
    free(tree);
    return 0;
}
