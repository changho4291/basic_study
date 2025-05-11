#include "memory_pool.h"
#include "generic_stack.h"
#include <stdio.h>

#define POOL_SIZE 1024
uint8_t pool_buffer[POOL_SIZE];

int main(int argc, char const *argv[]) {
    MemoryPool pool;
    memory_pool_init(&pool, pool_buffer, sizeof(GenericStack), 4);  // 스택 객체 풀

    MemoryPool data_pool;
    memory_pool_init(&data_pool, pool_buffer + 4 * sizeof(GenericStack), 64, 4);    // 데이터 풀

    // int 들어가는 경우
    GenericStack* s = stack_create(&pool, sizeof(int), 16); // 16개 init 스택
    if (!s) {
        printf("Failed to create stack\n");
        return 1;
    }

    for (int i = 1; i <= 5; i++) { stack_push(s, &i); }

    int x;
    while (stack_pop(s, &x)) {
        printf("Popped: %d\n", x);
    }
    
    stack_destroy(&pool, s);
    
    // 데이터 풀 해제
    memory_pool_free(&data_pool, pool_buffer + 4 * sizeof(GenericStack));

    typedef struct DataStruct {
        int firstData;
        char secondData[2];
    } DataStruct;

    memory_pool_init(&data_pool, pool_buffer + 4 * sizeof(GenericStack), sizeof(DataStruct), 20);    // 데이터 풀 재 할당

    // DataStruct 들어가는 경우
    GenericStack* s2 = stack_create(&pool, sizeof(DataStruct), 20); // 20개 DataStruct 스택
    if (!s2) {
        printf("Failed to create stack\n");
        return 1;
    }

    DataStruct ds1 = {
        .firstData = 1,
        .secondData = "a"
    };

    DataStruct ds2 = {
        .firstData = 2,
        .secondData = "b"
    };

    DataStruct ds3 = {
        .firstData = 3,
        .secondData = "c"
    };

    stack_push(s2, &ds1);
    stack_push(s2, &ds2);
    stack_push(s2, &ds3);

    DataStruct x2;
    while (stack_pop(s, &x2)) {
        printf("Popped: firstData - %d, secondData - %s\n", x2.firstData, x2.secondData);
    }
    
    stack_destroy(&pool, s2);

    // 풀 해제
    memory_pool_free(&pool, pool_buffer);
    memory_pool_free(&data_pool, pool_buffer + 4 * sizeof(GenericStack));

    return 0;
}
