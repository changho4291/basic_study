#include <iostream>
#include <cstdint>

using namespace std;

int main(int argc, char const *argv[]) {
    cin.tie(0); cout.tie(0);
    ios_base::sync_with_stdio(0);
    
    int n;
    uint32_t result = 0;
    cin >> n;

    int cStart = (n + 2) / 3;
    int cEnd = (n - 1) / 2;

    for (int c = cStart; c <= cEnd; c++) {
        int s = n - c; //(a + b)

        int b_low = (s + 1) / 2;            // B의 하한
        int b_high = min(c, s - 1);     // B의 상한

        if (b_low <= b_high) {result += (b_high - b_low + 1);}
    }

    cout << result << "\n";
    return 0;
}