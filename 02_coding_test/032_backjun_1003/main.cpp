#include <cstdio>
#include <vector>

int fibonacci(int n) {
    if (n == 0) {
        return 0;
    } else if (n == 1) {
        return 1;
    } else {
        return fibonacci(n-1) + fibonacci(n-2);
    }
}

int main(int argc, char const *argv[]) {
    int t;
    scanf("%d", &t);
    std::vector<int> zero_arr;
    zero_arr.reserve(t);

    std::vector<int> one_arr;
    one_arr.reserve(t);
    
    for (int i = 0; i < t; i++) {
        int n;

        scanf("%d", &n);
        int zero = 0, one = 0;

        if (fibonacci(n)) { one++;}
        else { zero++; }

        printf(" %d %d\n", zero, one);
        zero_arr[i] = zero;
        one_arr[i] = one;
    }

    for (int i = 0; i < t; i++) {
        printf("%d %d\n", zero_arr[i], one_arr[i]);
    }
}