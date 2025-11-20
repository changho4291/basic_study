#include <iostream>
#include <vector>
#include <queue>
#include <tuple>

using namespace std;

int main(int argc, char const *argv[]) {
    cin.tie(0);
    cout.tie(0);
    ios_base::sync_with_stdio(0);

    int n, m;
    cin >> n >> m;

    int moveX[4] = {0, 1, 0, -1};
    int moveY[4] = {1, 0, -1, 0};
    vector<vector<bool>> isVisited(n, vector<bool>(m, false));
    vector<vector<int>> board(n, vector<int>(m));
    vector<vector<int>> result(n, vector<int>(m, 0));
    queue<tuple<int, int, int>> bfs;
    pair<int, int> start;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            cin >> board[i][j];
            if (board[i][j] == 2)  { 
                start.first = i;
                start.second = j;
                isVisited[i][j] = true;
                bfs.push({i, j, 0});
            }
        }
    }

    while (!bfs.empty()) {
        tuple<int, int, int> current = bfs.front();
        bfs.pop();

        for (int i = 0; i < 4; i++) {
            int x = get<0>(current) + moveX[i];
            int y = get<1>(current) + moveY[i];

            if (x < 0 || y < 0 || x >= n || y >= m) { continue; }
            if (board[x][y] == 0 || isVisited[x][y]) { continue; }

            isVisited[x][y] = true;
            bfs.push({x, y, get<2>(current) + 1});
            result[x][y] = get<2>(current) + 1;
        }
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (board[i][j] == 1 && result[i][j] == 0) {
                cout << "-1";    
            } else {
                cout << result[i][j];
            }
            
            if (j !=  m - 1) {
                cout << " ";
            }
        }
        cout << "\n";
    }
    return 0;
} 