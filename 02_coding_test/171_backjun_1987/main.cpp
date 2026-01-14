#include <iostream>
#include <vector>
#include <queue>

using namespace std;

typedef struct {
    int x;
    int y;
    int cnt;
} Data;

int main(int argc, char const *argv[]) {
    int r, c;

    cin >> r >> c;
    vector<string> board(r, string(c, ' '));
    vector<vector<bool>> isVisited(r, vector<bool>(c, false));
    vector<bool> alphaUsed(26, false);
    queue<Data> bfs;

    int moveX[] = {0, 1, 0, -1};
    int moveY[] = {1, 0, -1, 0};

    for (int i = 0; i < r; i ++) {
        for (int j = 0; j < c; j++) {
            cin >> board[i][j];
        }
    }

    bfs.push({0, 0, 1});
    isVisited[0][0] = true;
    alphaUsed[board[0][0] - 'A'] = true;

    int result = 0;
    while (!bfs.empty()) {
        auto current = bfs.front();
        bfs.pop();

        for (int i = 0; i < 4; i++) {
            int x = current.x + moveX[i];
            int y = current.y + moveY[i];

            if (x < 0 || y < 0 || x >= r || y >= r) { continue; }
            if (alphaUsed[board[x][y] - 'A'] || isVisited[x][y]) { continue; }

            bfs.push({x, y, current.cnt + 1});
            isVisited[x][y] = true;
            alphaUsed[board[x][y] - 'A'] = true;
            result = current.cnt + 1;
        }
    }
    
    cout << result << "\n";
    return 0;
}

