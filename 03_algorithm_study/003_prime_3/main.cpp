#include <iostream>
#include <cstdint>

// 1000 개의 수 중에 소수를 구하기 3 (최적화 2번)
// 소수가 아닌 수로 나누는 경우는 의미가 없다.
// 그러므로 이미 소수로 판명된 수를 이용해서만 나눗셈을 해서 판단
// 여기에서 더 나아가서 비교하고자 하는 수가 소수의 제곱근 까지만 비교를 해서 연산을 하면
// 그 이상은 더이상 연산을 해볼 필요가 없다는 원리를 이용한다. 즉
// 5 X 5 = 25다. 25가 소수인지 확인하려면 결국엔 5 이상을 확인 해볼 필요가 없다.
// 마찬가지로 27이 소수인지 확인하기 위해 5 x 5 = 25 즉 5로 나누어서 나누어 떨어지지 않으면
// 7 x 7 = 49 까지 가는 7 위로는 굳이 나눗셈을 할 필요가 없이 27은 소수라고 판명되는 것 이다.

int main(int argc, char const *argv[]) {
    uint64_t counter = 0;               // 나눗셈 횟수

    int prime[500] = {0};
    int ptr = 0;

    prime[ptr++] = 2;
    prime[ptr++] = 3;

    for(int n = 5; n <= 1000; n += 2) { // 홀수만을 대상
        int i;
        int flag = 0;                   // 결과가 소수인지 아닌지 확인하는 플래그
        for (i = 1; counter++, prime[i] * prime[i] <= n; i++) {     // 소수인지 판별하고자 하는 값 n 을 소수 * 소수 까지만 비교
            counter++;
            if (n % prime[i] == 0) {    // 나누어 떨어지면 소수가 아님
                flag = 1;
                break; 
            }
        }
        if (!flag) {                 // 마지막가지 나누어떨어지지 않았다면
            prime[ptr++] = n;        // 배열에 저장
            printf("%d\n", n); 
        }
    }
    printf("나눗셈을 실행한 횟수: %lu\n", counter);

    return 0;
}
