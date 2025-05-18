#include "thread_tree.h"
#include <stdio.h>
#include <stdlib.h>

static TNode* create_node(int data) {
    TNode* node = (TNode*)malloc(sizeof(TNode));
    node->data = data;
    node->left = node->right = NULL;
    node->isLeftThread = true;
    node->isRightThread = true;
    return node;
}

// 삽입 (중위 스레드 유지)
TNode* ttree_insert(TNode* root, int data) {
    if (!root) { return create_node(data); }

    TNode* parent = NULL;
    TNode* curr = root;

    while (curr) {
        if (data == curr->data) { return root; }    // 중복은 무시

        parent = curr;

        if (data < curr->data) {
            if (!curr->isLeftThread) { curr = curr->left; }
            else { break; }
        } else {
            if (!curr->isRightThread) { curr = curr->right; }
            else { break; }
        }
    }

    TNode* new_node = create_node(data);
    if (data < parent->data) {
        new_node->left = parent->left;
        new_node->right = parent;
        parent->isLeftThread = false;
        parent->left = new_node;
    } else {
        new_node->right = parent->right;
        new_node->left = parent;
        parent->isRightThread = false;
        parent->right = new_node;
    }

    return root;
}

// 중위 순회 (스택/재귀 없이)
void ttree_inorder(TNode* root) {
    TNode* curr = root;

    // 맨 왼쪽 노드로 이동
    while (curr && !curr->isLeftThread) {
        curr = curr->left;
    }

    while (curr) {
        printf("%d ", curr->data);

        if (curr->isRightThread) {
            curr = curr->right;
        } else {
            curr = curr->right;
            while (curr && !curr->isLeftThread) {
                curr = curr->left;
            }
        }
    }
}

// 노드 삭제
TNode* ttree_delete(TNode* root, int value) {
    TNode* parent = NULL;
    TNode* curr = root;
    bool found = false;

    // 노드 탐색
    while (curr) {
        if (value == curr->data) {
            found = true;
            break;
        }

        parent = curr;
        if (value < curr->data) {
            if (curr->isLeftThread) { break; }
            curr = curr->left;
        } else {
            if (curr->isRightThread) { break; }
            curr = curr->right;
        }
    }
    
    if (!found) { return root; }    // 값 없음

    // 1. 두 자식 모두 있는 경우 -> 중위 후계자로 치환
    if (!curr->isLeftThread && !curr->isRightThread) {
        TNode* succ = curr->right;
        TNode* succParent = curr;

        while (!succ->isLeftThread) {
            succParent = succ;
            succ = succ->left;
        }

        curr->data = succ->data;
        value = succ->data;
        parent = succParent;
        curr = succ;
    }

    // 2. 삭제 대상 노드가 리프 또는 단일 자식일 경우
    TNode* child = NULL;
    if (!curr->isLeftThread) { child = curr->left; }
    else if (!curr->isRightThread) {child = curr->right;}

    // 자식이 없으면 child는 NULL

    // 스레드 복구
    if (!curr->isLeftThread) {
        TNode* pred = curr->left;
        while (!pred->isRightThread) {
            pred = pred->right;
        }
        pred->right = curr->right;
    }
    if (!curr->isRightThread) {
        TNode* succ = curr->right;
        while (!succ->isLeftThread) {
            succ = succ->left;
        }
        succ->left = curr->left;
    }

    // 부모와 연결 끊기
    if (!parent) {
        // 삭제 대상이 루트
        free(curr);
        return child;
    }

    if (value < parent->data) {
        if (curr->isLeftThread && curr->isRightThread) {
            parent->left = curr->left;
            parent->isLeftThread = true;
        } else {
            parent->left = child;
        }
    } else {
        if (curr->isLeftThread && curr->isRightThread) {
            parent->right = curr->right;
            parent->isRightThread = true;
        } else {
            parent->right = child;
        }
    }

    free(curr);
    return root;
}

// 재귀적 메모리 해제 (스레드는 건너뜀)
void ttree_free(TNode* root) {
    if (!root) { return; }
    if (!root->isLeftThread) { ttree_free(root->left); }
    if (!root->isRightThread) { ttree_free(root->right); }
    free(root);
}