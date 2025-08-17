#include <stdio.h>

int main(int argc, char const *argv[]) {
    int n, temp, min, max;

    scanf("%d", &n);

    for (int i = 0; i < n; i++) {
        scanf("%d", &temp);

        if (i == 0) {
            min = temp;
            max = temp;
        }

        if (temp < min) { min = temp; }
        else if (temp > max) { max = temp; }
    }

    printf ("%d %d", min, max);
}