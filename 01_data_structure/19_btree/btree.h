#ifndef __BTREE_H__
#define __BTREE_H__

#define MIN_DEGREE 1    // t: 최소 차수

typedef struct BTreeNode {
    int keys[2 * MIN_DEGREE - 1];
    struct BTreeNode* children[2 * MIN_DEGREE];
    int num_keys;
    int leaf;
} BTreeNode;

typedef struct {
    BTreeNode* root;
} BTree;

// 트리 생성
BTree* btree_create();

// 삽입
void btree_insert(BTree* tree, int key);

// 순회
void btree_traverse(BTreeNode* node);

// 검색
BTreeNode* btree_search(BTreeNode* node, int key);

// 메모리 해제
void btree_free(BTreeNode* node);

#endif // __BTREE_H__