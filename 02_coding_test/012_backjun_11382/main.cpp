#include <stdio.h>
#include <stdint.h>

int main(int argc, char const *argv[]) {
    uint64_t a, b, c;

    scanf("%ld %ld %ld", &a, &b, &c);
    
    printf("%ld", a + b + c);
    return 0;
}