#include <iostream>
#include <vector>
#include <queue>
#include <cmath>

using namespace std;

typedef struct {
    int x;
    int y;
    int depth;
} node;


int main(int argc, char const *argv[]) {
    cin.tie(0);
    cout.tie(0);
    ios_base::sync_with_stdio(0);

    int moveX[4] = {0, 1, 0, -1};
    int moveY[4] = {1, 0, -1, 0};

    int n, r, d;
    pair<int, int> enumy;
    cin >> n >> r >> d >> enumy.first >> enumy.second;

    vector<pair<int, int>> towers(n);

    int maxX = enumy.first, maxY = enumy.second;
    for (auto& t : towers) {
        cin >> t.first >> t.second;
        if (maxX < t.first) { maxX = t.first; }
        if (maxY < t.second) { maxY = t.second; }
    }

    // 1은 적, 2는 타워
    vector<vector<int>> board(maxX + 1, vector<int>(maxY + 1, 0));
    vector<vector<bool>> visited(maxX + 1, vector<bool>(maxY + 1, false));
    queue<node> bfs;

    board[enumy.first][enumy.second] = 1;
    bfs.push({enumy.first, enumy.second, -1});
    visited[enumy.first][enumy.second] = true;

    for (auto& t : towers) {
        board[t.first][t.second] = 2;
    }
    
    float result = 0;
    while(!bfs.empty()) {
        auto current = bfs.front();
        bfs.pop();
        queue<pair<int, int>> subBfs;
        subBfs.push({current.x, current.y});

        while(!subBfs.empty()) {
            auto subCurrent = subBfs.front();
            subBfs.pop();


            for(int i = 0; i < 4; i++) {
                int x = subCurrent.first + moveX[i];
                int y = subCurrent.second + moveY[i];

                if (x < 0 || y < 0 || x > maxX || y > maxY) { continue; }
                if (abs(current.x - x) + abs(current.y - y) > r) { continue; }
                if (visited[x][y]) { continue; }

                if (board[x][y] == 2) {
                    bfs.push({x, y, current.depth + 1});
                    result += (float) d / (1 * pow(2, current.depth + 1));
                    subBfs.push({x, y});
                } else {
                    subBfs.push({x, y});
                }
                visited[x][y] = true;
            }
        }
    }

    cout << fixed;
    cout.precision(5);
    cout << result << "\n";

    return 0;
}
