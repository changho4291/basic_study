#include <iostream>
#include <queue>

using namespace std;

int main(int argc, char const *argv[]) {
    cin.tie(0);
    cout.tie(0);
    ios_base::sync_with_stdio(0);

    int t, xSize, ySize, n;
    int x, y;

    // 좌표 이동을 위한 배열 ()
    int dx[4] = {0, 1, 0, -1};
    int dy[4] = {1, 0, -1, 0};

    cin >> t;

    for (int i = 0; i < t; i++) {
        cin >> xSize >> ySize >> n;
        xSize++;
        ySize++;
        vector<vector<int>> filed(xSize, vector<int> (ySize, 0));
        vector<vector<bool>> visited(xSize, vector<bool> (ySize, false));
        queue<pair<int, int>> bsp;
        int cnt = 0;    // 지렁이 수

        for (int j = 0; j < n; j++) {
            cin >> x >> y;
            filed[x][y] = 1;
        }

        for (int j = 0; j < xSize; j++) {
            for (int k = 0; k < ySize; k++) {
                // 배추가 없다면 무시
                if (filed[j][k] == 0) { continue; }

                // 방문 했다면 무시
                if (visited[j][k]) { continue; }

                visited[j][k] = true;
                bsp.push({j, k});
                cnt++;

                while (!bsp.empty()) {
                    pair<int, int> current = bsp.front();
                    bsp.pop();
                    
                    for (int l = 0; l < 4; l++) {
                        int nextX = current.first + dx[l];
                        int nextY = current.second + dy[l];
                        
                        if (nextX < 0 || nextY < 0 || nextX >= xSize || nextY >= ySize) { continue; }
                        if (filed[nextX][nextY] == 0) { continue; }
                        if (visited[nextX][nextY]) { continue; }

                        visited[nextX][nextY] = true;
                        bsp.push({nextX, nextY});
                    }
                }
            }
        }
        cout << cnt << "\n";
    }
    
    return 0;
}