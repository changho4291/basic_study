#include <cstdio>

int main(int argc, char const *argv[]) {
    int n;
    scanf("%d", &n);

    int i = 1;
    while (true) {
        n -= i;

        if (n <= 0) {
            if ( (i % 2) == 0 ) {   // 짝수일때 분자 증가 분모 감소
                printf("%d/%d", i + n, i - ((i-1) + n));
            }
            else {                  // 홀수일때 분모 증가 분자 감소
                printf("%d/%d", i - ((i-1) + n), i + n);
            }
            break;
        }
        i++;
    }
}