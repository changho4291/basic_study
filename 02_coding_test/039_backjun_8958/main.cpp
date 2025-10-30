#include <iostream>

int main(int argc, char const *argv[]) {
    int T;
    std::string ox;
    
    std::cin >> T;
    
    for(int i = 0; i < T; i++) {
        std::cin >> ox;
        int score = 0;
        int adder = 0;
        for(int i = 0; i < ox.length(); i++) {
            if (ox[i] == 'O') {
                adder++;
                score += adder;
            } else {
                adder = 0;
            }
        }
        std::cout << score << "\n";
    }

    return 0;
}