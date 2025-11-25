#include <iostream>
#include <vector>
#include <queue>
#include <cstdint>

using namespace std;

int main(int argc, char const *argv[]) {
    
    int n, m;
    cin >> n >> m;

    int moveX[4] = {0, 1, 0, -1};
    int moveY[4] = {1, 0, -1, 0};

    vector<vector<int>> board(n, vector<int>(m));
    vector<vector<bool>> visited(n, vector<bool>(m, false));
    queue<pair<int, int>> bfs;

    // 종료조건1
    uint64_t nwjr = 0;

    for(int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            cin >> board[i][j];
            nwjr += board[i][j];
        }
    }

    bool isBlock = false;
    int year = 0;
    while (nwjr > 0) {
        for(int i = 0; i < n; i++) {
            int flag = false;
            for (int j = 0; j < m; j++) {
                if (board[i][j] > 0) {
                    bfs.push({i, j});
                    visited[i][j] = true;
                    flag = true;
                    break;
                }
            }
            if (flag) {break;}
        }

        while(!bfs.empty()) {
            pair<int, int> current = bfs.front();
            bfs.pop();

            for(int i = 0; i < 4; i++) {
                int x = current.first + moveX[i];
                int y = current.second + moveY[i];

                if (x < 0 || y < 0 || x >= n || y >= m) { continue; }
                if (visited[x][y]) { continue; }
                if (board[x][y] == 0) { // 바다와 인접하면 빙산 깍음
                    if (board[current.first][current.second] > 0) {
                        board[current.first][current.second]--;
                    }
                    continue;
                }

                bfs.push({x, y});
                visited[x][y] = true;
            }
        }

        nwjr = 0;
        // 여기서 나눠졌는지 체크
        for(int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                if (board[i][j] > 0 && visited[i][j] == false) {    // 섬 분단 종료조건 충족
                    cout << year << "\n";
                    return 0;
                }
                visited[i][j] = false;
                nwjr += board[i][j];
            }
        }

        // 이 1회전이 1년
        year++;
    }
    
    cout << "0\n";

    return 0;
}
