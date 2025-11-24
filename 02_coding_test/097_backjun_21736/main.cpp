#include <iostream>
#include <vector>
#include <queue>

using namespace std;

int main(int argc, char const *argv[]) {
    cin.tie(0);
    cout.tie(0);
    ios_base::sync_with_stdio(0);

    int moveX[4] = {0, 1, 0, -1};
    int moveY[4] = {1, 0, -1, 0};

    int n, m;
    cin >> n >> m;

    vector<vector<char>> campus(n, vector<char>(m));
    vector<vector<bool>> visited(n, vector<bool>(m, false));
    queue<pair<int, int>> bfs;

    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            cin >> campus[i][j];

            if (campus[i][j] == 'I') {
                bfs.push({i, j});
                visited[i][j] = true;
            }
        }
    }

    int cnt = 0;
    while(!bfs.empty()) {
        pair<int, int> current = bfs.front();
        bfs.pop();

        for(int i = 0; i < 4; i++) {
            int x = current.first + moveX[i];
            int y = current.second + moveY[i];

            if (x < 0 || y < 0 || x >= n || y >= m) { continue; }
            if (campus[x][y] == 'X' || visited[x][y] == true) { continue; }

            bfs.push({x, y});
            visited[x][y] = true;
            if (campus[x][y] == 'P') { cnt++; }
        }
    }

    if (cnt == 0) {
        cout << "TT\n";
        return 0;
    }

    cout << cnt << "\n";

    return 0;
} 