#include <iostream>
#include <cstdint>

using namespace std;

int main(int argc, char const *argv[]) {
    cin.tie(0); cout.tie(0);
    ios_base::sync_with_stdio(0);
    
    uint64_t n, b;
    cin >> n >> b;

    b = (1ll << (b+1)) - 1;

    if (b >= n) {
        cout << "yes\n";
    } else {
        cout << "no\n";
    }

    return 0;
}