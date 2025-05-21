#include "btree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static BTreeNode* btree_create_node(int leaf) {
    BTreeNode* node = (BTreeNode*)malloc(sizeof(BTreeNode));
    node->leaf = leaf;
    node->num_keys = 0;
    memset(node->children, 0, sizeof(node->children));
    return node;
}

// 노드 분할 (full child y -> split to y and z)
static void btree_split_child(BTreeNode* parent, int idx, BTreeNode* y) {
    BTreeNode* z = btree_create_node(y->leaf);
    z->num_keys = MIN_DEGREE - 1;

    for (int j = 0; j < MIN_DEGREE - 1; j++) {
        z->keys[j] = y->keys[j + MIN_DEGREE];
    }
    if (!y->leaf) {
        for (int j = 0; j < MIN_DEGREE; j++) {
            z->children[j] = y->children[j + MIN_DEGREE];
        }
    }

    y->num_keys = MIN_DEGREE - 1;

    for (int j = parent->num_keys; j >= idx + 1; j--) {
        parent->children[j + 1] = parent->children[j];
    }
    parent->children[idx + 1] = z;

    for (int j = parent->num_keys - 1; j >= idx; j--) {
        parent->keys[j + 1] = parent->keys[j];
    }
    parent->keys[idx] = y->keys[MIN_DEGREE - 1];
    parent->num_keys += 1;
}

static void btree_insert_nonfull(BTreeNode* node, int key) {
    int i = node->num_keys - 1;

    if (node->leaf) {
        while (i >= 0 && key < node->keys[i]) {
            node->keys[i + 1] = node->keys[i];
            i--;
        }
        node->keys[i + 1] = key;
        node->num_keys++;
    } else {
        while (i >= 0 && key < node->keys[i]) {
            i--;
        }
        i++;
        if (node->children[i]->num_keys == 2 * MIN_DEGREE - 1) {
            btree_split_child(node, i, node->children[i]);
            if (key > node->keys[i]) { i++; }
        }
        btree_insert_nonfull(node->children[i], key);
    }
}

BTree* btree_create() {
    BTree* tree = (BTree*)malloc(sizeof(BTree));
    tree->root = btree_create_node(1);
    return tree;
}

void btree_insert(BTree* tree, int key) {
    BTreeNode* r = tree->root;
    if (r->num_keys == 2 * MIN_DEGREE - 1) {
        BTreeNode* s = btree_create_node(0);
        tree->root = s;
        s->children[0] = r;
        btree_split_child(s, 0 ,r);
        btree_insert_nonfull(s, key);
    } else {
        btree_insert_nonfull(r, key);
    }
}

void btree_traverse(BTreeNode* node) {
    if (!node) { return; }
    for (int i = 0; i < node->num_keys; i++) {
        if (!node->leaf) { btree_traverse(node->children[i]); }
        printf("%d ", node->keys[i]);
    }
    if (!node->leaf) { btree_traverse(node->children[node->num_keys]); }
}

BTreeNode* btree_search(BTreeNode* node, int key) {
    int i = 0;
    while (i < node->num_keys && key > node->keys[i]) { i++; }
    if (i < node->num_keys && key == node->keys[i]) { return node; }
    if (node->leaf) { return NULL; }
    return btree_search(node->children[i], key);
}

void btree_free(BTreeNode* node) {
    if (!node) { return; }
    if (!node->leaf) {
        for (int i = 0; i <= node->num_keys; i++) {
            btree_free(node->children[i]);
        }
    }
    free(node);
}