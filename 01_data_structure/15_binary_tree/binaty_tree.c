#include "binary_tree.h"
#include <stdio.h>
#include <stdlib.h>

TreeNode* bt_create(int value) {
    TreeNode* node = (TreeNode*)malloc(sizeof(TreeNode));
    node->data = value;
    node->left = node->right = NULL;
    return node;
}

TreeNode* bt_insert(TreeNode* root, int value) {
    if (!root) { return bt_create(value); }

    if (value < root->data) {
        root->left = bt_insert(root->left, value);
    } else if (value > root->data) {
        root->right = bt_insert(root->right, value);
    }

    return root;
}

bool bt_search(TreeNode* root, int value) {
    if (!root) { return false; }
    if (value == root->data) { return true; }
    if (value < root->data) {
        return bt_search(root->left, value);
    } else {
        return bt_search(root->right, value);
    }
}

void bt_inorder(TreeNode* root) {
    if (!root) { return; }
    bt_inorder(root->left);
    printf("%d ", root->data);
    bt_inorder(root->right);
}

void bt_preorder(TreeNode* root) {
    if (!root) { return; }
    printf("%d ", root->data);
    bt_preorder(root->left);
    bt_preorder(root->right);
}

void bt_postorder(TreeNode* root) {
    if (!root) { return; }
    bt_postorder(root->left);
    bt_postorder(root->right);
    printf("%d ", root->data);
}

void bt_free(TreeNode* root) {
    if (!root) { return; }
    bt_free(root->left);
    bt_free(root->right);
    free(root);
}