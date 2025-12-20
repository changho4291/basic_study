#include <iostream>
#include <vector>
#include <queue>

using namespace std;

typedef struct {
    int x; 
    int y;
    int d;  // 순회 깊이
    bool b;  // 벽을 부쉈는지
} Data;

int main () {
    cin.tie(0);
    cout.tie(0);
    ios_base::sync_with_stdio(0);

    int moveX[] = {0, 0, -1, 1};
    int moveY[] = {-1, 1, 0, 0};

    int n, m;
    cin >> n >> m;

    vector<vector<int>> board(n, vector<int>(m));
    vector<vector<vector<bool>>> isVisited(n, vector<vector<bool>>(m, vector<bool>(1, false)));

    queue<Data> bfs;
    
    for(int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            char t;
            cin >> t;
            board[i][j] = t - '0';
        }
    }

    if (n == 1 && m == 1) { 
        cout << "1\n"; 
        return 0;
    }

    bfs.push({0, 0, 1, 0});
    isVisited[0][0][0] = true;

    while (!bfs.empty()) {
        auto current = bfs.front();
        bfs.pop();

        for (int i = 0; i < 4; i++) {
            int x = current.x + moveX[i];
            int y = current.y + moveY[i];

            if (x < 0 || y < 0 || x >= n || y >= m) { continue; }
            if (isVisited[x][y][current.b] || ((board[x][y] != 0) && (current.b != 0)) ) { continue; }
            
            if ((x == n -1) && (y == m-1)) {
                cout << current.d + 1 << "\n";
                return 0;
            }

            if (board[x][y] != 0) {
                bfs.push({x, y, current.d + 1, true});
                isVisited[x][y][true] = true;
            } else {
                bfs.push({x, y, current.d + 1, current.b});
                isVisited[x][y][current.b] = true;
            }
        }
    }
    cout << "-1\n";

    return 0;
}