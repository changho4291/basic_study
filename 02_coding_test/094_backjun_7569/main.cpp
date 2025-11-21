#include <iostream>
#include <vector>
#include <queue>
#include <tuple>

using namespace std;

typedef struct {
    int x;
    int y;
    int z;
    int depth;
} Position;

int main(int argc, char const *argv[]) {
    cin.tie(0);
    cout.tie(0);
    ios_base::sync_with_stdio(0);

    int moveZ[6] = {0, 0, 0, 0, 1, -1};
    int moveY[6] = {0, 1, 0, -1, 0, 0};
    int moveX[6] = {1, 0, -1, 0, 0, 0};

    int a, b, c;
    cin >> a >> b >> c;
    vector<vector<vector<int>>> tomato(c, vector<vector<int>>(b, vector<int>(a)));
    vector<vector<vector<bool>>> visited(c, vector<vector<bool>>(b, vector<bool>(a, false)));
    queue<Position> bfs;
    
    for (int i = 0; i < c; i++) {
        for (int j = 0; j < b; j++) {
            for (int k = 0; k < a; k++) {
                cin >> tomato[i][j][k];
                if (tomato[i][j][k] == 1) {
                    bfs.push({k, j, i, 0});
                    visited[i][j][k] = true;
                }
            }
        }
    }

    int result = 0;
    while (!bfs.empty()) {
        Position current = bfs.front();
        bfs.pop();

        for(int i = 0; i < 6; i++) {
            int x = current.x + moveX[i];
            int y = current.y + moveY[i];
            int z = current.z + moveZ[i];

            if (x < 0 || y < 0 || z < 0 || x >= a || y >= b || z >= c) { continue; }
            if (tomato[z][y][x] != 0 || visited[z][y][x] == true) { continue; }

            bfs.push({x, y, z, current.depth + 1});
            visited[z][y][x] = true;
            result = current.depth + 1;
        }
    }

    // 모두 익지 못한 상황 확인
    for (int i = 0; i < c; i++) {
        for (int j = 0; j < b; j++) {
            for (int k = 0; k < a; k++) {
                if (!visited[i][j][k] && tomato[i][j][k] == 0) {
                    cout << "-1\n";
                    return 0;
                }
            }
        }
    }

    cout << result << "\n";

    return 0;
} 