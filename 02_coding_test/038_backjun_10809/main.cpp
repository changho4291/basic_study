#include <iostream>

int main(int argc, char const *argv[]) {
    int arr[26];
    std::string S;

    for (int i = 0; i < 26; i++) {
        arr[i] = -1;
    }

    std::cin >> S;

    for(int i = 0; i < S.length(); i++) {
        if (arr[S[i] - 'a'] == -1 ) {
            arr[S[i] - 'a'] = i;
        }
    }

    for (int i = 0; i < 26; i++) {
        std::cout << arr[i] << " ";
    }
    std::cout << "\n";

    return 0;
}