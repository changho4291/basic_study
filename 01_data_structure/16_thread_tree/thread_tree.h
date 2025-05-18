#ifndef __THREAD_TREE_H__
#define __THREAD_TREE_H__

#include <stdbool.h>

typedef struct TNode {
    int data;
    struct TNode* left;
    struct TNode* right;
    bool isLeftThread;
    bool isRightThread;
} TNode;

// 트리 생성
TNode* ttree_insert(TNode* root, int value);

// 중위 순회 (비재귀)
void ttree_inorder(TNode* root);

// 노드 삭제
TNode* ttree_delete(TNode* root, int value);

// 메모리 해제
void ttree_free(TNode* root);

#endif // __THREAD_TREE_H__