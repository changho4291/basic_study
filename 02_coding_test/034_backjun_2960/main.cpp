// 에라토스테네스의 체

#include <cstdio>

int main(int argc, char const *argv[]) {
    int n, k;
    scanf("%d %d", &n, &k);

    int prime[500];
    int ptr = 0;    // 소수 배열 증가
    int cnt = 1;    // 삭제 순번 증가
    prime[ptr++] = 2;

    for (int i = 2; i <= n; i++) {
        int flag = 0;
        for (int j = 0; prime[j] * prime[j] <= i; j++) {
            if (i % prime[j] == 0) {   // 나누어 떨어지면 소수가 아님!
                flag = 1;
                break;
            }
        }
        if (!flag) {
            prime[ptr++] = i;
            // TODO 여기에서 나눠야하네 그런데 그 조건을 다 찾으면
            // 속도가 느려지지 않나??
            cnt++;
            printf("%d\n", i); 
        }

        if(cnt == k) { 
            printf("%d\n", i);
            break;
        }
    }

    // printf("소수의 수 %d", ptr);

    return 0;
}