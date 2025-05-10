#ifndef __GENERIC_STACK_H__
#define __GENERIC_STACK_H__

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct Stack Stack;

// 메모리 해제용 함수 포인터
typedef void (*FreeFunc)(void*);

// 출력용 (디버깅)
typedef void (*PrintFunc)(const void*);

Stack* stack_create(size_t elem_size, FreeFunc free_fn);
void stack_destroy(Stack* s);

int stack_push(Stack* s, const void* elem);
int stack_pop(Stack* s, void* out);
int stack_peek(Stack* s, void* out);
int stack_is_empty(const Stack* s);

void stack_print(const Stack* s, PrintFunc print_fn);

#ifdef __cplusplus
}
#endif

#endif // __GENERIC_STACK_H__