#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main(int argc, char const *argv[]) {
    cin.tie(0);
    cout.tie(0);
    ios_base::sync_with_stdio(0);

    int t, n;
    cin >> t;

    for (int i = 0; i < t; i++) {
        cin >> n;

        vector<int> dp(n);

        dp[0] = 1;
        dp[1] = 2;
        dp[2] = 4;

        for (int i = 3; i < n; i++) {
            dp[i] = dp[i-1] + dp[i-2] + dp[i-3];
        }

        cout << dp[n - 1] << "\n";
    }

    return 0;
}