#include <iostream>

using namespace std;

int main(int argc, char const *argv[]) {
    cin.tie(0);
    ios_base::sync_with_stdio(0);

    int n, m;
    int min = 0;
    int max;

    cin >> n >> m;

    max = n * m;

    while (!min) {
        if ( n > m ) {
            n = n % m;
        } else {
            m = m % n;
        }

        if (n == 0) { min = m; }
        else if ( m == 0) { min = n; }
    }

    max /= min;

    cout << min << "\n";
    cout << max << "\n";
    
    return 0;
}