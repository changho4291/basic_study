#include <iostream>
#include <vector>
#include <queue>

using namespace std;

typedef struct {
    int x;
    int y;
    int moveCnt;
} Shark;

int main(int argc, char const *argv[]) {
    cin.tie(0); cout.tie(0);
    ios_base::sync_with_stdio(0);
    
    int moveX[] = {-1, 0, 0, 1};
    int moveY[] = {0, -1, 1, 0};

    int n;
    cin >> n;

    vector<vector<int>> map(n, vector<int>(n));
    queue<Shark> bfs;

    // 디버깅용
    vector<vector<int>> traking(n, vector<int>(n, 0));
    int secno = 1;

    int startX, startY, size = 2, eatCnt = 0;
    int result = 0;
    // 맵 설정
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cin >> map[i][j];
            if (map[i][j] == 9) {
                
                startX = i;
                startY = j;
                map[i][j] = 0;
            }
        }
    }

    while (true) {
        vector<vector<bool>> visited(n, vector<bool>(n, false));
        bfs.push({startX, startY, 0});

        int bestD = 1 << 30;
        int bestX = -1, bestY = -1;

        while (!bfs.empty()) {
            Shark current = bfs.front();
            bfs.pop();

            for (int i = 0; i < 4; i++) {
                int x = current.x + moveX[i];
                int y = current.y + moveY[i];
                int d = current.moveCnt + 1;

                if (x < 0 || y < 0 || x >= n || y >= n || map[x][y] > size || visited[x][y]) { continue; }

                // 먹이를 찾았을때
                if (map[x][y] != 0 && size > map[x][y]) { 
                    if (d < bestD || (d == bestD && (x < bestX || (x == bestX && y < bestY)))) { 
                        bestD = d; 
                        bestX = x;
                        bestY = y;
                    }
                }

                bfs.push({x, y, d});
                visited[x][y] = true;
                traking[x][y] = d;
            }
        }

        if (bestX == -1) break;

        result += bestD;
        startX = bestX;
        startY = bestY;
        map[bestX][bestY] = 0;
        eatCnt++;
        if (eatCnt == size) {
            size++;
            eatCnt = 0;
        }
    }
    
    
    cout << result << "\n";

    return 0;
}