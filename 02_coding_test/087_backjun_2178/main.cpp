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

    vector<string> board(n);
    vector<vector<bool>> visited(n, vector<bool>(m, false));
    queue<tuple<int, int, int>> bfs;

    int moveX[4] = {0, 1, 0, -1};
    int moveY[4] = {1, 0, -1, 0};

    for(auto& b : board) {
        cin >> b;
    }

    bfs.push({0, 0, 1});
    visited[0][0] = true;
    int result = 0;
    
    while(!bfs.empty()) {
        tuple<int, int, int> current = bfs.front();
        bfs.pop();

        for(int i = 0; i < 4; i++) {
            int x = get<0>(current) + moveX[i];
            int y = get<1>(current) + moveY[i];

            if (x < 0 || x >= n || y < 0 || y >= m || board[x][y] == '0') { continue; }
            if (board[x][y] == '0' || visited[x][y]) { continue; }

            if (x == n - 1 && y == m - 1) {
                result = get<2>(current) + 1;
                break;
            }

            visited[x][y] = true;
            bfs.push({x, y, get<2>(current) + 1});
        }
        if (result != 0) { break; }
    }

    cout << result << "\n";

    return 0;
}