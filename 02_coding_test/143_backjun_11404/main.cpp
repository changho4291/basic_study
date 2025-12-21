#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

#define INF 0x7FFFFFFF

int main () {
    cin.tie(0);
    cout.tie(0);
    ios_base::sync_with_stdio(0);

    int n, m;
    cin >> n >> m;

    vector<vector<int>> floydTable(n, vector<int>(n, INF));

    int j = INF;

    int a, b, c;
    for (int i = 0; i < m; i++) {
        cin >> a >> b >> c;
        if (floydTable[a - 1][b - 1] > c) { floydTable[a- 1][b - 1] = c; }
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            for (int k = 0; k < n; k++) {
                if (j == k) { floydTable[j][k] = 0; continue;}
                if (floydTable[j][i] == INF || floydTable[i][k] == INF) { continue; }
                floydTable[j][k] = min(floydTable[j][i] + floydTable[i][k], floydTable[j][k] );
            }
        }
    }
    
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (floydTable[i][j] == INF) { floydTable[i][j] = 0; }
            cout << floydTable[i][j] << " ";
        }
        cout << "\n";
    }

    return 0;
}