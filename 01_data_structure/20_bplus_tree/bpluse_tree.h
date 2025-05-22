#ifndef __BPLUSE_TREE_H__
#define __BPLUSE_TREE_H__

#define DEGREE 3

typedef struct BPlusNode {
    int is_leaf;
    int num_keys;
    int keys[DEGREE];
    int values[DEGREE];                     // 리프 노드에만 사용
    struct BPlusNode* children[DEGREE + 1]; // 내부 노드에만 사용
    struct BPlusNode* next;                 // 리프 노드 연결용
} BPlusNode;

// 전역 루트 노드
extern BPlusNode* root;

// 트리 조작 함수
void insert(int key, int value);
void delete_key(int key);
void print_dot_tree(void);

#endif // __BPLUSE_TREE_H__