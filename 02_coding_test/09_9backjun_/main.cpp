#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

int main(int argc, char const *argv[]) {
    cin.tie(0);
    cout.tie(0);
    ios_base::sync_with_stdio(0);

    int moveX[4] = {0, 1, 0, -1};
    int moveY[4] = {1, 0, -1, 0};

    int n;
    cin >> n;

    vector<vector<char>> aptMap(n, vector<char>(n));
    vector<vector<bool>> visited(n, vector<bool>(n, false));
    queue<pair<int, int>> bfs;

    for(int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cin >> aptMap[i][j];
        }
    }

    int cnt = 0;    // 단지 수
    vector<int> result;
    for(int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (aptMap[i][j] == '1' && visited[i][j] == false) {
                bfs.push({i, j});
                visited[i][j] = true;
                cnt++;
                int temp = 1;   // 가구 수
                while(!bfs.empty()) {
                    pair<int, int> current = bfs.front();
                    bfs.pop();

                    for(int i = 0; i < 4; i ++) {
                        int x = current.first + moveX[i];
                        int y = current.second + moveY[i];

                        if (x < 0 || y < 0 || x >= n || y >= n) { continue; }
                        if (aptMap[x][y] == '0' || visited[x][y] == true) { continue; }

                        bfs.push({x, y});
                        visited[x][y] = true;
                        temp++;
                    }
                }
                result.push_back(temp);
            }
        }
    }

    sort(result.begin(), result.end());

    cout << cnt << "\n";
    
    for (const int& i : result) {
        cout << i << "\n";
    }

    return 0;
} 