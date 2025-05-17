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

// 서브트리 중 가장 왼쪽 노드 찾기
static TreeNode* bt_min_value_node(TreeNode* node) {
    TreeNode* current = node;
    while (current && current->left) {
        current = current->left;
    }

    return current;
}

// 노드 삭제
TreeNode* bt_delete(TreeNode* root, int value) {
    if (!root) { return NULL; }

    if (value < root->data) {
        root->left = bt_delete(root->left, value);
    } else if (value > root->data) {
        root->right = bt_delete(root->right, value);
    } else {
        // Case 1, 2: one or zero child
        if (!root->left) {
            TreeNode* temp = root->right;
            free(root);
            return temp;
        } else if (!root->right) {
            TreeNode* temp = root->left;
            free(root);
            return temp;
        }

        // Case 3: tow children
        TreeNode* successor = bt_min_value_node(root->right);
        root->data = successor->data;
        root->right = bt_delete(root->right, successor->data);
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

int bt_count_nodes(TreeNode* root) {
    if (!root) { return 0; }
    return 1 + bt_count_nodes(root->left) + bt_count_nodes(root->right);
}

int bt_count_leaves(TreeNode* root) {
    if (!root) { return 0; }
    if (!root->left && !root->right) { return 1; }
    return bt_count_leaves(root->left) + bt_count_leaves(root->right);
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