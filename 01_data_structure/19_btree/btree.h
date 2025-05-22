#ifndef __BTREE_H__
#define __BTREE_H__

#define MIN_DEGREE 2    // t: 최소 차수

/**
 * @brief 
| 필드         | 설명                                |
| ------------ | --------------------------------- |
| `keys[]`     | 정렬된 키 값 저장 (최대 `2t - 1`개)         |
| `children[]` | 각 키 구간을 담당하는 자식 포인터 (`keys + 1`개) |
| `num_keys`   | 현재 사용 중인 키의 개수                    |
| `leaf`       | 리프 노드 여부 (`1: 단말 노드`, `0: 내부 노드`) |
 * 
 */

typedef struct BTreeNode {
    int keys[2 * MIN_DEGREE - 1];               // 노드가 보유한 키 배열 (최대 2t - 1개의 키를 가짐)
    struct BTreeNode* children[2 * MIN_DEGREE]; // 자식 노드를 가리키는 포인터 배열 (최대 2t개의 자식 가능)
    int num_keys;                               // 현재 노드에 실제로 저장된 키의 개수
    int leaf;                                   // 현재 노드가 리프 노드인지 여부 (1 = leaf, 0 = 내부 노드)
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