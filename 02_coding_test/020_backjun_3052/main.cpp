#include <stdio.h>

int main(int argc, char const *argv[]) {
    int n, cnt;
    int remain[10];

    for (int i = 0; i < 10; i++) {
        scanf("%d", &n);
        remain[i] = n % 42;

        if (i == 0) { cnt = 1; }
        else {
            bool find_same = false;
            for (int j = 0; j < i; j++) {
                if (remain[i] == remain[j]) { 
                    find_same = true;
                    break;
                }
            }
            if ( !find_same ) { cnt++; }
        }
    }

    printf("%d", cnt);
}