#include <iostream>
#include <vector>

using namespace std;

#define INF 0x7FFFFFFF

int main () {
    cin.tie(0);
    cout.tie(0);
    ios_base::sync_with_stdio(0);

    int n, m;
    cin >> n >> m;
    vector<vector<int>> floydTable(n, vector<int>(n, INF));
    vector<vector<int>> trackingTable(n, vector<int>(n, 0));

    int a, b, c;
    for (int i = 0; i < m; i++) {
        cin >> a >> b >> c;
        if (floydTable[a - 1][b - 1] > c) {
            floydTable[a - 1][b - 1] = c;
            trackingTable[a - 1][b - 1] = b;
        }
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            for (int k = 0; k < n; k++) {
                if (floydTable[j][i] == INF || floydTable[i][k] == INF || j == k) { continue; }
                if (floydTable[j][k] > floydTable[j][i] + floydTable[i][k]) {
                    floydTable[j][k] = floydTable[j][i] + floydTable[i][k];
                    trackingTable[j][k] = trackingTable[j][i];
                }
            }
        }
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i == j || floydTable[i][j] == INF) { floydTable[i][j] = 0; }
            cout << floydTable[i][j] << " ";
        }
        cout << "\n";
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int tmp = trackingTable[i][j];
            if (tmp == 0) {
                cout << "0\n";
                continue;
            }

            int k = 2;
            vector<int> course;
            course.push_back(i + 1);
            course.push_back(tmp);

            while (tmp != (j + 1) && tmp != 0) {
                k++;
                tmp = trackingTable[tmp - 1][j];
                course.push_back(tmp);
            }
            cout << k << " ";
            for (int& l : course) { cout << l << " "; }
            cout << "\n";
        }
    }

    return 0;
}