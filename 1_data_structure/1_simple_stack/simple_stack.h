#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_SIZE 100

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    int data[MAX_SIZE];
    int top;
} Stack;

/**
 * @brief 스택 초기화
 * 
 * @param s 자료형
 */
void init(Stack* s);

/**
 * @brief 비어 있는지 확인
 * 
 * @param s 자료형
 * @return true 
 * @return false 
 */
bool is_empty(Stack* s);

/**
 * @brief 가득 찼는지 확인
 * 
 * @param s 자료형
 * @return true 
 * @return false 
 */
bool is_full(Stack* s);

/**
 * @brief 삽입
 * 
 * @param s 자료형
 * @param value 삽입할 값
 * @return true 
 * @return false 
 */
bool push(Stack* s, int value);

/**
 * @brief 제거
 * 
 * @param s 자료형
 * @param out 출력된 값
 * @return true 
 * @return false 
 */
bool pop(Stack* s, int* out);

/**
 * @brief 최상단 값 확인
 * 
 * @param s 자료형
 * @param out 출력된 값
 * @return true 
 * @return false 
 */
bool peek(Stack* s, int* out);

#ifdef __cplusplus
}
#endif