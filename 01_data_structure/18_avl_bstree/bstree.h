#ifndef __BSTREE_H__
#define __BSTREE_H__

typedef struct Node {
    int data;
    int height;
    struct Node* left;
    struct Node* right;
} Node;

// 삽입
Node* bst_insert(Node* root, int value);

// 검색
int bst_serch(Node* root, int value);

// 중위 순회
void bst_inorder(Node* root);

// 메모리 해제
void bst_free(Node* root);

#endif // __BSTREE_H__