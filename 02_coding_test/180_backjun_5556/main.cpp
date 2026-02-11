#include <iostream>

using namespace std;

int main(int argc, char const *argv[]) {
    cin.tie(0); cout.tie(0);
    ios_base::sync_with_stdio(0);

    int n, k;
    cin >> n >> k;

    int x, y;
    for (int i = 0; i < k; i++) {
        cin >> x >> y;

        if (x > n/2) { x = n - x + 1; }
        if (y > n/2) { y = n - y + 1; }

        if (x == y || x < y) {
            if (x % 3) { cout << x % 3 << "\n"; }
            else { cout << "3\n"; }
        } else {
            if (y % 3) { cout << y % 3 << "\n"; }
            else { cout << "3\n"; }
        }
    }

    return 0;
}