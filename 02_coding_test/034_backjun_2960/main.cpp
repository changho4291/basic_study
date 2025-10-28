// 에라토스테네스의 체

#include <cstdio>

int main(int argc, char const *argv[]) {
    int n, k;
    scanf("%d %d", &n, &k);

    int cnt = 0;    // 삭제 순번 증가

    // n이 1보다 작거나 같으면 종료
    if (n <= 1) { return 0; }

    bool prime[1001];
    for(int i = 0; i <= n; i++) {
        prime[i] = true;
    }

    for (int i = 2; i <= n; i++) {
        if(prime[i]) {
            cnt++;
            if (cnt == k) { printf("%d\n", i); }

            for(int j = i * i; j <= n; j += i) {
                if(!prime[j]) { continue; }
                cnt++;
                prime[j] = false;
                if (cnt == k) { printf("%d\n", j); }
            }

            if (cnt >= k) {break;}
        }
    }

    return 0;
}