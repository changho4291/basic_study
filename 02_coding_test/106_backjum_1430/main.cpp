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
    queue<tuple<int, int, int>> bfs;

    board[enumy.first][enumy.second] = 1;
    bfs.push({enumy.first, enumy.second, 0});
    visited[enumy.first][enumy.second] = true;

    for (auto& t : towers) {
        board[t.first][t.second] = 2;
    }
    
    while(!bfs.empty()) {
        auto current = bfs.front();
        bfs.pop();

        for(int i = 0; i < 4; i++) {
            int x = get<0>(current) + moveX[i];
            int y = get<1>(current) + moveY[i];

            if (x < 0 || y < 0 || x > maxX || y > maxY) { continue; }
            if (visited[x][y]) { continue; }

            if (board[x][y] == 2 && (get<2>(current) + 1) < r) {
                bfs.push({x, y, 0});
            } else {
                bfs.push({x, y, get<2>(current) + 1});
            }
        }
    }

    return 0;
}
