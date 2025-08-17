#include <stdio.h>

int main(int argc, char const *argv[]) {
    int h, m;
    scanf("%d %d", &h, &m);

    m -= 45;
    if (m < 0) {
        m += 60;
        if (h == 0) { h = 23; } 
        else { h--; }
    }

    printf("%d %d", h, m);

    return 0;
}