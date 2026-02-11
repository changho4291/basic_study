#include <iostream>
#include <vector>
#include <cstdint>

using namespace std;

int main(int argc, char const *argv[]) {
    cin.tie(0); cout.tie(0);
    ios_base::sync_with_stdio(0);

    int n, m, j = 0;
    cin >> n >> m;
    vector<int> z(m + 1);
    z[0] = 0;

    int64_t a, b;

    for (int i = 1; i <= m; i++) {
        cin >> a >> b;

        if (a == 1) {
            j = (j + (b % n)) % n;
        } else if (a == 2) {
            j = (j - (b % n) + n) % n;
        } else {
            j = z[b];
        }

        z[i] = j;
    }
    
    cout << z[m] + 1 << "\n";
    return 0;
}