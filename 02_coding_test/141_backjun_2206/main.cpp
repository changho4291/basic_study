#include <iostream>
#include <vector>
#include <queue>

using namespace std;

typedef struct {
    int x; 
    int y;
    int d; 
    char s;
} Data;

int main () {
    cin.tie(0);
    cout.tie(0);
    ios_base::sync_with_stdio(0);

    int moveX[] = {0, 0, -1, 1};
    int moveY[] = {-1, 1, 0, 0};

    int n, m;
    cin >> n >> m;

    vector<vector<char>> board(n, vector<char>(m));
    vector<vector<bool>> isVisited(n, vector<bool>(m, false));
    queue<Data> bfs;

    for(auto& v : board) {
        for (char& c : v) {
            cin >> c;
        }
    }

    bfs.push({0, 0, 1, '1'});
    isVisited[0][0] = true;

    while (!bfs.empty()) {
        auto current = bfs.front();
        bfs.pop();

        for (int i = 0; i < 4; i++) {
            int x = current.x + moveX[i];
            int y = current.y + moveY[i];

            if (x < 0 || y < 0 || x >= n || y >= m) { continue; }
            if (isVisited[x][y] || ((board[x][y] == '1') && (current.s == '0')) ) { continue; }
            
            if ((x == n -1) && (y == m-1)) {
                cout << current.d + 1 << "\n";
                return 0;
            }

            if (board[x][y] == '1') {
                bfs.push({x, y, current.d + 1, '0'});
            } else {
                bfs.push({x, y, current.d + 1, current.s});
            }
            isVisited[x][y] = true;
        }
    }
    cout << "-1\n";

    return 0;
}