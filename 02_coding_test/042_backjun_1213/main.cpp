#include <iostream>

// 모든 문자가 짝수: 무조건 가능
// 홀수개인 문자 패턴이 2개 이상이면 불가능

int main(int argc, char const *argv[]) {
    char alpha[26];
    char odd_alpha;
    int num_of_odd = 0;

    for (int i = 0; i < 26; i++) {
        alpha[i] = 0;
    }

    std::string name;
    std::cin >> name;

    for(int i = 0; i < name.length(); i++) {
        alpha[name[i] - 'A']++;
    }

    for (int i = 0; i < 26; i++) {
        if (alpha[i] % 2 !=0) {
            num_of_odd++;
            odd_alpha = 'A' + i;
        }

        if (num_of_odd > 1) {
            std::cout << "I'm Sorry Hansoo\n";
            return 0;
        }
    }

    for(int i = 0; i < 26; i++) {
        for(int j = 0; j < alpha[i] / 2; j++) {
            printf("%c", 'A' + i);
        }
    }

    if (num_of_odd) {
        printf("%c", odd_alpha);
    }

    for(int i = 25; i >= 0; i--) {
        for(int j = 0; j < alpha[i] / 2; j++) {
            printf("%c", 'A' + i);
        }
    }

    printf("\n");

    return 0;
}