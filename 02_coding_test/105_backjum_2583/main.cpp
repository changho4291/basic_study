#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>

using namespace std;

int main(int argc, char const *argv[]) {
    cin.tie(0);
    cout.tie(0);
    ios_base::sync_with_stdio(0);

    int moveX[4] = {0, 1, 0, -1};
    int moveY[4] = {1, 0, -1, 0};
    
    int M, N, K;
    cin >> M >> N >> K;

    vector<vector<int>> board(M, vector<int>(N, 0));
    vector<vector<bool>> visited(M, vector<bool>(N, false));
    stack<pair<int, int>> dfs;

    for (int i = 0; i < K; i++) {
        int x1, y1, x2, y2;

        cin >> x1 >> y1 >> x2 >> y2;

        for (int j = y1; j < y2; j++) {
            for (int k = x1; k < x2; k++) {
                board[j][k] = 1;
            }
        }
    }

    int result = 0;
    vector<int> resultVector;
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            if (board[i][j] == 0 && !visited[i][j]) {
                int cnt = 0;
                result++;
                dfs.push({i, j});
                while (!dfs.empty()) {
                    auto current = dfs.top();
                    dfs.pop();

                    if (visited[current.first][current.second] 
                        || board[current.first][current.second] == 1) { continue; }

                    visited[current.first][current.second] = true;
                    cnt++;

                    for(int i = 0; i < 4; i++) {
                        int x = current.first + moveX[i];
                        int y = current.second + moveY[i];

                        if (x < 0 || y < 0 || x >= M || y >= N) { continue; }

                        dfs.push({x, y});
                    }
                }
                resultVector.push_back(cnt);
            }
        }
    }

    sort(resultVector.begin(), resultVector.end());

    cout << result << "\n";
    for (auto& r : resultVector) {
        cout << r << " ";
    }
    cout << "\n";

    return 0;
}
