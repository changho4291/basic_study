#include <iostream>
#include <cstdint>

using namespace std;

int main(int argc, char const *argv[]) {
    cin.tie(0);
    cout.tie(0);
    ios_base::sync_with_stdio(0);

    int n;
    cin >> n;

    for (int i = 0; i < n; i++) {
        uint64_t result = 0;
        string binaryStr;
        cin >> binaryStr;
        for (int j = 23; j >= 0; j--) {
            result |= (binaryStr[j] - '0') << 23 - j;
        }
        cout << result << "\n";
    }
    return 0;
}