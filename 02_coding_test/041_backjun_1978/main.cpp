#include <iostream>

int main(int argc, char const *argv[]) {
    int prime[1001];

    prime[0] = 0;
    prime[1] = 0;
    for (int i = 2; i < 1001; i++) {
        prime[i] = 1;
    }

    for(int i = 2; i < 1001 ; i++) {
        for(int j = 2; j * j <= i ; j++) {
            if( i % j == 0 ) {
                prime[i] = 0;
                break;
            }
        }
    }

    int N, cnt = 0;
    int num;

    std::cin >> N;

    for (int i = 0; i < N; i++) {
        std::cin >> num;

        if(prime[num]) { cnt++; }
    }

    std::cout << cnt << "\n";

    return 0;
}