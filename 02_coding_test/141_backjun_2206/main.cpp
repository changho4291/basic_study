#include <iostream>
#include <vector>
#include <queue>

using namespace std;

typedef struct {
    int x; 
    int y;
    int d;
} Data;

int main () {
    cin.tie(0);
    cout.tie(0);
    ios_base::sync_with_stdio(0);

    int moveX[] = {0, 0, -1, 1};
    int moveY[] = {-1, 1, 0, 0};

    int n, m;
    cin >> n >> m;

    vector<vector<char>> board(n, vector<char>(m, 0));
    vector<pair<int, int>> walls;
    queue<Data> bfs;

    for(int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            cin >> board[i][j];
            if (board[i][j] == '1') {
                walls.push_back({i, j});
            }
        }
    }

    int result = -1;
    for (auto& w : walls) {
        board[w.first][w.second] = '0';
        vector<vector<bool>> isVisited(n, vector<bool>(m, false));
        bfs.push({0, 0, 1});
        isVisited[0][0] = true;

        bool isRun = true;
        while (!bfs.empty()) {
            auto current = bfs.front();
            bfs.pop();

            for (int i = 0; i < 4; i++) {
                int x = current.x + moveX[i];
                int y = current.y + moveY[i];

                if (x < 0 || y < 0 || x >= n || y >= m) { continue; }
                if (isVisited[x][y] || (board[x][y] == '1')) { continue; }
                
                if ((x == n -1) && (y == m-1)) {
                    if ( (result == -1) || result > current.d + 1 ) { result = current.d + 1; }
                    isRun = false;
                    break;
                }

                bfs.push({x, y, current.d + 1});
                isVisited[x][y] = true;
            }

            if(!isRun) { break; }
        }
        board[w.first][w.second] = '1';
    }

    cout << result << "\n";

    return 0;
}