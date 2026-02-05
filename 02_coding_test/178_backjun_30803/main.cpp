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

    int q, c, i, x;
    cin >> q;

    for (int j =  0; j < q; j++) {
        cin >> c;

        if (c == 2) {
            cin >> i;
            isUse[i-1] = !isUse[i-1];

            if (isUse[i-1]) { r += a[i-1]; }
            else { r -= a[i-1]; }
            cout << r << "\n";
        } else {
            cin >> i >> x;
            int tmp = a[i - 1];
            a[i - 1] = x;

            if (isUse[i-1]) { 
                r += (a[i-1] - tmp);
            }
            cout << r << "\n";
        }
    }

    return 0;
}