#include "bstree.h"
#include <stdio.h>
#include <stdlib.h>

static int max(int a, int b) { return (a > b) ? a : b; }

static int height(Node* n) { return n ? n->height : 0; }

static Node* new_node(int value) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->data = value;
    node->height = 1;
    node->left = node->right = NULL;
    return node;
}

// 회전
static Node* rotate_right(Node* y) {
    Node* x = y->left;
    Node* T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    return x;
}

static Node* rotate_left(Node* x) {
    Node* y = x->right;
    Node* T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    return y;
}

// 군형 인수 계산 
static int get_balance(Node* n) {
    return n ? height(n->left) - height(n->right) : 0;
}

Node* bst_insert(Node* root, int value) {
    if (!root) { return new_node(value); }

    if (value < root->data) {
        root->left = bst_insert(root->left, value);
    } else if (value > root->data) {
        root->right = bst_insert(root->right, value);
    } else {
        return root;    // 중복 무시
    }

    root->height = max(height(root->left), height(root->right)) + 1;
    int balance = get_balance(root);

    // LL
    if (balance > 1 && value < root->left->data) {
        return rotate_right(root);
    }
    // RR
    if (balance < -1 && value > root->right->data) {
        return rotate_left(root);
    }
    // LR
    if (balance > 1 && value > root->left->data) {
        root->left = rotate_left(root->left);
        return rotate_right(root);
    }
    // RL
    if (balance < -1 && value < root->right->data) {
        root->right = rotate_right(root->right);
        return rotate_left(root);
    }

    return root;
}

int bst_serch(Node* root, int value) {
    while (root) {
        if (value == root->data) { return 1; }
        root = (value < root->data) ? root->left : root->right;
    }
    return 0;
}

void bst_inorder(Node* root) {
    if (!root) { return; }
    bst_inorder(root->left);
    printf("%d ", root->data);
    bst_inorder(root->right);
}

void bst_free(Node* root) {
    if (!root) { return; }
    bst_free(root->left);
    bst_free(root->right);
    free(root);
}