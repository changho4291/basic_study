#include <stdio.h>

int main(int argc, char const *argv[]) {
    int n;

    scanf("%d", &n);

    float subjects[1000];
    float max = 0, sum = 0;

    for (int i = 0; i < n; i++) {
        scanf("%f", &subjects[i]);
        if ( max < subjects[i]) { max = subjects[i]; }
    }

    for (int i = 0; i < n; i++) {
        subjects[i] = subjects[i] / max * 100;
        sum += subjects[i];
    }

    printf("%lf", sum/n);
}