#ifndef __BINARY_TREE_H__
#define __BINARY_TREE_H__

#include <stdbool.h>

typedef struct TreeNode {
    int data;
    struct TreeNode* left;
    struct TreeNode* right;
} TreeNode;

// 노드 생성
TreeNode* bt_create(int value);

// 노드 삽입 (BST 규칙)
TreeNode* bt_insert(TreeNode* root, int value);

// 노드 삭제
TreeNode* bt_delete(TreeNode* root, int value);

// 값 탐색
bool bt_search(TreeNode* root, int value);

// 전체 노드 수 반환
int bt_count_nodes(TreeNode* root);

// 잎 노드 수 반환
int bt_count_leaves(TreeNode* root);

// 순회
void bt_inorder(TreeNode* root);
void bt_preorder(TreeNode* root);
void bt_postorder(TreeNode* root);

// 트리 메모리 해제
void bt_free(TreeNode* root);

#endif // __BINARY_TREE_H__