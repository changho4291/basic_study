#include <iostream>
#include <vector>
#include <queue>
#include <tuple>

using namespace std;

int main(int argc, char const *argv[]) {
    cin.tie(0);
    cout.tie(0);
    ios_base::sync_with_stdio(0);

    int moveX[4] = {0, 1, 0, -1};
    int moveY[4] = {1, 0, -1 ,0};

    int m, n;
    cin >> m >> n;

    vector<vector<int>> box(n,vector<int>(m));
    vector<vector<bool>> visited(n, vector<bool>(m, false));
    queue<tuple<int, int, int>> bfs;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            cin >> box[i][j];
            if (box[i][j] == 1) {
                bfs.push({i, j, 0});
                visited[i][j] = true;
            }
        }
    }

    int cnt = 0;
    while(!bfs.empty()) {
        tuple<int, int, int> current = bfs.front();
        bfs.pop();

        for (int i = 0; i < 4; i++) {
            int x = get<0>(current) + moveX[i];
            int y = get<1>(current) + moveY[i];

            if (x < 0 || y < 0 || x >= n || y >= m) { continue; }
            if (box[x][y] == -1 || visited[x][y] == true) { continue; }

            bfs.push({x, y, get<2>(current) + 1});
            visited[x][y] = true;
            cnt = get<2>(current) + 1;
        }
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (box[i][j] == 0 && visited[i][j]  == false) {
                cout << "-1\n";
                return 0;
            }
        }
    }

    cout << cnt << "\n";

    return 0;
} 