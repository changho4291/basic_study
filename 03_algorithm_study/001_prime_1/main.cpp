#include <iostream>
#include <cstdint>

// 1000 개의 수 중에 소수를 구하기 1 (최적화 되어 있지 않은 경우)
// 단순하게 모든 수를 나누어서 1과 자기자신으로만 나누어지는지를 판단 하는 방법

int main(int argc, char const *argv[]) {
    uint64_t counter = 0;
    for(int n = 2; n <= 1000; n++) {
        int i;
        for (i = 2; i < n; i++) {
            counter++;
            if (n % i == 0) { break; }
        }
        if (n == i) { printf("%d\n", n); }
    }
    printf("나눗셈을 실행한 횟수: %lu\n", counter);

    return 0;
}
