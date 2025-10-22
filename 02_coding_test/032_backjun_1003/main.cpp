#include <cstdio>
#include <vector>

int main(int argc, char const *argv[]) {
    int t, n, fibo[41] = {0, 1};

    for(int i = 2; i < 41; i++) {
        fibo[i] = fibo[i - 2] + fibo[i - 1];
    }

    scanf("%d", &t);
    std::vector<int> zero_arr;
    zero_arr.reserve(t);

    std::vector<int> one_arr;
    one_arr.reserve(t);
    
    for (int i = 0; i < t; i++) {
        scanf("%d", &n);

        if (!n) {
            zero_arr[i] = 1;
        } else {
            zero_arr[i] = fibo[n-1];
        }

        one_arr[i] = fibo[n];
    }

    for (int i = 0; i < t; i++) {
        printf("%d %d\n", zero_arr[i], one_arr[i]);
    }
}