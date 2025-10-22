#include <cstdio>
#include <vector>
#include <cstdint>

int fibonacci(int n, uint32_t& zero, uint32_t& one) {
    if (n == 0) {
        zero++;
        return 0;
    } else if (n == 1) {
        one++;
        return 1;
    } else {
        return fibonacci(n-1, zero, one) + fibonacci(n-2, zero, one);
    }
}

int main(int argc, char const *argv[]) {
    int t;
    scanf("%d", &t);
    std::vector<uint32_t> zero_arr;
    zero_arr.reserve(t);

    std::vector<uint32_t> one_arr;
    one_arr.reserve(t);
    
    for (int i = 0; i < t; i++) {
        int n;

        scanf("%d", &n);

        fibonacci(n, zero_arr[i], one_arr[i]);
    }

    for (int i = 0; i < t; i++) {
        printf("%d %d\n", zero_arr[i], one_arr[i]);
    }
}