#include <cstdio>
int main(int argc, char const *argv[]) {
    int n, m;
    int cards[100];

    scanf("%d %d", &n, &m);

    for(int i = 0; i < n; i++) { scanf("%d", &cards[i]); }

    int same = 0;
    for (int i = 0; i < n - 2; i++) {
        for (int j = i + 1; j < n - 1; j++) {
            for (int k = j + 1; k < n; k++) {
                int sum = cards[i] + cards[j] + cards[k];

                if (m - sum == 0) { same = sum; }
                if (m - same > m - sum && m - sum >= 0) {
                    same = sum;
                }
            }
        }
    }

    printf("%d", same);
}