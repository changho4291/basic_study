#include <iostream>

int main(int argc, char const *argv[]) {
    int T, R;
    std::string S;

    std::cin >> T;
    
    for(int i = 0; i < T; i++) {
        std::cin >> R >> S;
        for(int j = 0; j < S.length(); j++) {
            for(int i = 0; i < R; i++) {
                std::cout << S.c_str()[j];
            }
        }
        std::cout << "\n";
    }

    return 0;
}