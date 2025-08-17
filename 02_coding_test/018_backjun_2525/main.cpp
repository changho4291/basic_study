#include <stdio.h>

int main(int argc, char const *argv[]) {
    int a, b, c;
    scanf("%d %d", &a, &b);
    scanf("%d", &c);

    a += (b + c) / 60;
    a %= 24;
    b = (b + c) % 60;

    printf("%d %d", a, b);
}