#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdint>

using namespace std;

int main(int argc, char const *argv[]) {
    cin.tie(0); cout.tie(0);
    ios_base::sync_with_stdio(0);

    uint64_t r, a, temp = 0, temp2 = 0;
    cin >> r;

    for (int i = 1; i <= r; i++) { 
        cin >> a;
        temp += a;
        temp2 += i;
    }
    temp2 -= r;

    cout << temp - temp2 << "\n";

    return 0;
}