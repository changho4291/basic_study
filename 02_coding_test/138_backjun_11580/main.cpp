#include <iostream>
#include <vector>
#include <cstdint>
#include <algorithm>

using namespace std;

int main () {
    cin.tie(0);
    cout.tie(0);
    ios_base::sync_with_stdio(0);

    int n, result = 0;
    cin >> n;
    vector<int> milk(n);

    for(auto& m : milk) { cin >> m; }

    sort(milk.begin(), milk.end());
    reverse(milk.begin(), milk.end());

    for(int i = 1; i <= n; i++) {
        if (i % 3 == 0) { continue; }
        result += milk[i - 1];
    }

    cout << result << "\n";

    return 0;
}