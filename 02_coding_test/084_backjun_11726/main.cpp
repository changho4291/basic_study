#include <iostream>
#include <vector>

using namespace std;

int main(int argc, char const *argv[]) {
    cin.tie(0);
    cout.tie(0);
    ios_base::sync_with_stdio(0);

    int n;
    cin >> n;
    
    vector<int> dp(n, 0);

    dp[0] = 1;
    dp[1] = 2;

    for (int i = 2; i < n; i++) {
        dp[i] = (dp[i - 1] + dp[i - 2]) % 10007;
    }

    cout << dp[n - 1] << "\n";

    return 0;
}