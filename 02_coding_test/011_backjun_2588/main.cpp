#include <stdio.h>

int main(int argc, char const *argv[]) {
    int first, second;

    scanf("%d", &first);
    scanf("%d", &second);
    
    printf("%d\n", first * (second % 10));
    printf("%d\n", first * ((second % 100) / 10));
    printf("%d\n", first * (second / 100));
    printf("%d", first * second);
    return 0;
}
