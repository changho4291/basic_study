#include <iostream>
#include <vector>
#include <queue>

using namespace std;

int main(int argc, char const *argv[]) {
    cin.tie(0);
    cout.tie(0);
    ios_base::sync_with_stdio(0);

    int n = 0;
    cin >> n;

    int moveX[4] = {0, 1, 0, -1};
    int moveY[4] = {1, 0, -1, 0};

    vector<vector<int>> sector(n, vector<int>(n));

    int maxNum = 0;
    int minNum = 1000;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cin >> sector[i][j];
            if (maxNum < sector[i][j]) { maxNum = sector[i][j]; }
            if (minNum > sector[i][j]) { minNum = sector[i][j]; }
        }
    }

    if (minNum == maxNum) {
        cout << "1\n";
        return 0;
    }

    int result = 0;
    for (int a = minNum; a < maxNum; a++) {
        queue<pair<int, int>> bfs;
        vector<vector<bool>> visited(n, vector<bool>(n, false));

        int cnt = 0;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (visited[i][j]) { continue; }
                if( sector[i][j] <= a ) { continue; }
                bfs.push({i, j});
                cnt++;

                while (!bfs.empty()) {
                    auto current = bfs.front();
                    bfs.pop();

                    for (int k = 0; k < 4; k++) {
                        int x = current.first + moveX[k];
                        int y = current.second + moveY[k];

                        if (x < 0 || y < 0 || x >= n || y >= n) { continue; }
                        if (sector[x][y] <= a || visited[x][y]) { continue; }
                        
                        visited[x][y] = true;
                        bfs.push({x, y});
                    }
                }
            }
        }

        if (result < cnt) { result = cnt; }
    }

    cout << result << "\n";
    
    return 0;
}