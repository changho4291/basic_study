#include <cstdio>

int main(int argc, char const *argv[]) {
    int a1, a0, c, n0, result;
    scanf("%d %d", &a1, &a0);
    scanf("%d", &c);
    scanf("%d", &n0);

    int fn = (a1 * n0) + a0;
    int fn2 = a1 * n0;
    int g = c * n0;

    if (fn <= g && fn2 <= g) { result = 1; }
    else { result = 0; }

    printf("%d", result);
}