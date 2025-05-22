#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bpluse_tree.h"

BPlusNode* root = NULL;

static BPlusNode* create_node(int is_leaf) {
    BPlusNode* node = calloc(1, sizeof(BPlusNode));
    node->is_leaf = is_leaf;
    return node;
}

static void insert_internal(int key, BPlusNode* right_child, BPlusNode* parent);
static void insert_leaf(BPlusNode* leaf, int key, int value);
static void split_leaf(BPlusNode* leaf, int key, int value);
static void delete_from_leaf(BPlusNode* leaf, int key);

// 삽입
void insert(int key, int value) {
    if (!root) {
        root = create_node(1);
        insert_leaf(root, key, value);
        return;
    }

    BPlusNode* node = root;
    while (!node->is_leaf) {
        int i = 0;
        while (i < node->num_keys && key >= node->keys[i]) { i++; }
        node = node->children[i];
    }

    if (node->num_keys < DEGREE) {
        insert_leaf(node, key, value);
    } else {
        split_leaf(node, key, value);
    }
}

static void insert_leaf(BPlusNode* leaf, int key, int value) {
    int i = leaf->num_keys - 1;
    while (i >= 0 && leaf->keys[i] > key) {
        leaf->keys[i + 1] = leaf->keys[i];
        leaf->values[i + 1] = leaf->values[i];
        i--;
    }
    leaf->keys[i + 1] = key;
    leaf->values[i + 1] = value;
    leaf->num_keys++;
}

static void split_leaf(BPlusNode* leaf, int key, int value) {
    int temp_keys[DEGREE + 1];
    int temp_values[DEGREE + 1];
    for (int i = 0; i < DEGREE; i++) {
        temp_keys[i] = leaf->keys[i];
        temp_values[i] = leaf->values[i];
    }

    int i = DEGREE - 1;
    while (i >= 0 && key < temp_keys[i]) {
        temp_keys[i + 1] = temp_keys[i];
        temp_values[i + 1] = temp_values[i];
        i--;
    }
    temp_keys[i + 1] = key;
    temp_values[i + 1] = value;

    int split = (DEGREE + 1) / 2;
    leaf->num_keys = split;
    for (i = 0; i < split; i++) {
        leaf->keys[i] = temp_keys[i];
        leaf->values[i] = temp_values[i];
    }

    BPlusNode* new_leaf = create_node(1);
    new_leaf->num_keys = DEGREE + 1 - split;
    for (i = 0; i < new_leaf->num_keys; i++) {
        new_leaf->keys[i] = temp_keys[split + i];
        new_leaf->values[i] = temp_values[split + i];
    }

    new_leaf->next = leaf->next;
    leaf->next = new_leaf;

    if (leaf == root) {
        root = create_node(0);
        root->keys[0] = new_leaf->keys[0];
        root->children[0] = leaf;
        root->children[1] = new_leaf;
        root->num_keys = 1;
    } else {
        insert_internal(new_leaf->keys[0], new_leaf, root);
    }
}

static void insert_internal(int key, BPlusNode* child, BPlusNode* node) {
    if (node->is_leaf) { return; }

    int i = 0;
    while (i < node->num_keys && key >= node->keys[i]) { i++; }

    BPlusNode* target = node->children[i];
    if (target->is_leaf && target->num_keys < DEGREE) {
        insert_leaf(target, key, 0);
    } else {
        insert_internal(key, child, target);
    }
}

// 삭제
void delete_key(int key) {
    BPlusNode* node = root;
    while (!node->is_leaf) {
        int i = 0;
        while (i < node->num_keys && key >= node->keys[i]) { i++; }
        node = node->children[i];
    }
    delete_from_leaf(node, key);
}

static void delete_from_leaf(BPlusNode* leaf, int key) {
    int i;
    for (i = 0; i < leaf->num_keys; i++) {
        if (leaf->keys[i] == key) { break; }
    }
    if (i == leaf->num_keys) { return; }

    for (; i < leaf->num_keys - 1; i++) {
        leaf->keys[i] = leaf->keys[i + 1];
        leaf->values[i] = leaf->values[i + 1];
    }
    leaf->num_keys--;
}

// DOT 출력
static void print_node_dot(FILE* out, BPlusNode* node, int* id) {
    int node_id = (*id)++;
    fprintf(out, " node%d [label=\"", node_id);
    for (int i = 0; i < node->num_keys; i++) {
        fprintf(out, "<f%d> %d", i, node->keys[i]);
        if (i != node->num_keys - 1) { fprintf(out, "|"); }
    }
    fprintf(out, "\", shape=record];\n");

    if (!node->is_leaf) {
        for (int i = 0; i <= node->num_keys; i++) {
            int child_id = *id;
            print_node_dot(out, node->children[i], id);
            fprintf(out, " node%d -> node%d;\n", node_id, child_id);
        }
    }

    if (node->is_leaf && node->next) {
        fprintf(out, " node%d -> node%d [style=dotted constraint=false];\n", node_id, *id);
    }
}

void print_dot_tree(void) {
    FILE* out = fopen("bplus_tree.dot", "w");
    if (!out) {
        perror("fopen");
        return;
    }

    fprintf(out, "digraph BPlusTree {\n");
    fprintf(out, " node [shape=record];\n");

    int id = 0;
    print_node_dot(out, root, &id);

    fprintf(out, "}\n");
    fclose(out);
    printf("DOT 파일 생성 완료: bplus_tree.dot\n");
}