#include <iostream>
#include <cstdint>

using namespace std;

int main(int argc, char const *argv[]) {
    cin.tie(0);
    cout.tie(0);
    ios_base::sync_with_stdio(0);

    uint64_t a, b, fac;
    cin >> a >> b;

    if (a > b) {
        cout << "0\n";
        return 0;
    }

    fac = a;
    for (uint64_t i = a - 1; i > 0; i--) {
        fac = (fac * i) % b;
    }

    cout << fac << "\n";
    return 0;
}
