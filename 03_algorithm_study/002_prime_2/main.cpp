#include <iostream>
#include <cstdint>

// 1000 개의 수 중에 소수를 구하기 2 (최적화 1번)
// 소수가 아닌 수로 나누는 경우는 의미가 없다.
// 그러므로 이미 소수로 판명된 수를 이용해서만 나눗셈을 해서 판단

int main(int argc, char const *argv[]) {
    uint64_t counter = 0;               // 나눗셈 횟수

    int prime[500] = {0};
    int ptr = 0;

    prime[ptr++] = 2;

    for(int n = 3; n <= 1000; n += 2) { // 홀수만을 대상
        int i;
        for (i = 1; i < ptr; i++) {     // 이미 얻은 소수로 나눔
            counter++;
            if (n % prime[i] == 0) { break; }
        }
        if (ptr == i) {                 // 마지막가지 나누어떨어지지 않았다면
            prime[ptr++] = n;           // 배열에 저장
            printf("%d\n", n); 
        }
    }
    printf("나눗셈을 실행한 횟수: %lu\n", counter);

    return 0;
}
