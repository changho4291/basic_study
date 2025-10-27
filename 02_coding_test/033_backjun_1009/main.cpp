#include <cstdio>

int main(int argc, char const *argv[]) {
    int t, a, b;
    scanf("%d", &t);

    for (int i = 0; i < t; i++) {
        scanf("%d %d", &a, &b);

        int r = 1;
        for (int j = 0; j < b; j++) {
            r = (r*a) % 10;
        }

        if (r) { printf("%d\n", r); }
        else { printf("10\n"); }
    }

    return 0;
}