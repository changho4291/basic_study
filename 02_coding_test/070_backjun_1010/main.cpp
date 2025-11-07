#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

int main(int argc, char const *argv[]) {
    cin.tie(0);
    cout.tie(0);
    ios_base::sync_with_stdio(0);

    int t;
    cin >> t;

    for (int i = 0; i < t; i++) {
        int n, m;
        cin >> n >> m;

        vector<int> accum(m - (n - 1));

        int sum = 1;
        accum[0] = 1;

        for (int j = 1; j < m - (n - 1); j++) {
            // accum[i] = accum[i-1] + j + 1;
            // sum += accum[i];
        }

        cout << sum << "\n";
    }
    
    return 0;
}

/*
1 1     1
1 2     2
2 2     1
2 3     3
2 4     6
*/