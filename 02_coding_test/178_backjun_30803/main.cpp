#include <iostream>
#include <cstdint>
#include <vector>

using namespace std;

int main(int argc, char const *argv[]) {
    cin.tie(0); cout.tie(0);
    ios_base::sync_with_stdio(0);
    
    int n;
    uint64_t r = 0;
    cin >> n;

    vector<int> a(n);
    vector<bool> isUse(n, true);
    for(auto& v : a) { 
        cin >> v; 
        r += v;
    }

    cout << r << "\n";
    r = 0;

    int q, c, i, x;
    cin >> q;

    for (int j =  0; j < q; j++) {
        cin >> c;

        if (c == 2) {
            cin >> i;
            isUse[i-1] = !isUse[i-1];

            for(int k = 0; k < n; k++) { 
                if (isUse[k]) { r += a[k]; }
            }
            cout << r << "\n";
            r = 0;
        } else {
            cin >> i >> x;
            a[i - 1] = x;

            for(int k = 0; k < n; k++) { 
                if (isUse[k]) { r += a[k]; }
            }
            cout << r << "\n";
            r = 0;
        }
    }

    return 0;
}