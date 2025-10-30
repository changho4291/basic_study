#include <iostream>

int main(int argc, char const *argv[]) {
    int idx = 1;
    int num, max = 0;

    std::cin >> num;
    max = num;
    for (int i = 1; i < 9; i++) {
        std::cin >> num;
        if (max < num) {
            max = num;
            idx = i+1;
        }
    }

    std::cout << max << "\n" << idx << "\n";
    return 0;
}