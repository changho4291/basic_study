#include <iostream>
#include <cstdint>

using namespace std;

int main(int argc, char const *argv[]) {
    cin.tie(0);
    cout.tie(0);
    ios_base::sync_with_stdio(0);

    int t;
    cin >> t;

    for (int j = 0; j < t; j++) {
        int n, m;
        cin >> n >> m;

        uint64_t result = 1;

        for(int i = 0; i < n; i++) {
            result *= m-i;
            result /= 1+i;
        }

        cout << result << "\n";
    }
    
    return 0;
}