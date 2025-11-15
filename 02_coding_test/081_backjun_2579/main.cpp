#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main(int argc, char const *argv[]) {
    cin.tie(0);
    cout.tie(0);
    ios_base::sync_with_stdio(0);

    int n;
    cin >> n;

    vector<int> stairs(n);
    vector<int> dp;
    
    for (int& i : stairs) { cin >> i; }

    dp.push_back(stairs[0]);
    if (n >= 2) { dp.push_back(stairs[1] + stairs[0]); }
    if (n >= 3) { dp.push_back(max(stairs[2] + stairs[1], stairs[2] + stairs[0])); }

    for (int i = 3; i < n; i++) {
        dp.push_back(max(stairs[i] + stairs[i-1] + dp[i-3], stairs[i] + dp[i-2]));
    }
    
    cout << dp[n-1] << "\n";

    return 0;
}

// s1 =	r1

// s2 = r1 + r2

// s3 =	r1 + r3
// 	    r2 + r3

// s4 =	r1 + r2 + r4        = s2 + r4
// 	    r1 + r3 + r4	    = s1 + r4
// 	    r2 + r4		        = X

// s5 =	r1 + r2 + r4 + r5   = s2 + r4 + r5
// 	    r1 + r3 + r5	    = s3 + r5
// 	    r2 + r3 + r5	    = s3 + r5
// 	    r2 + r4 + r5	    = X

// s6 =	r1 + r2 + r4 + r6	= s4 + r6
// 	    r1 + r3 + r4 + r6	= s4 + r6
// 	    r1 + r3 + r5 + r6   = s3 + r5 + r6
// 	    r2 + r3 + r5 + r6	= s3 + r5 + r6
// 	    r2 + r4 + r6	=