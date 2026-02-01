#include <iostream>
#include <vector>
#include <queue>

using namespace std;

typedef struct {
    int size;
    int x;
    int y;
    int moveCnt;
    int eatCnt;
} Shark;

int main(int argc, char const *argv[]) {
    cin.tie(0); cout.tie(0);
    ios_base::sync_with_stdio(0);
    
    int moveX[] = {-1, 0, 0, 1};
    int moveY[] = {0, -1, 1, 0};

    int n;
    cin >> n;

    vector<vector<int>> map(n, vector<int>(n));
    vector<vector<bool>> visited(n, vector<bool>(n, false));
    queue<Shark> bfs;

    // 디버깅용
    vector<vector<int>> traking(n, vector<int>(n, 0));
    int secno = 1;

    // 맵 설정
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cin >> map[i][j];
            if (map[i][j] == 9) {
                bfs.push({2, i, j, 0, 0});
                visited[i][j] = true;
                map[i][j] = 0;
            }
        }
    }

    int result = 0;
    while (!bfs.empty()) {
        Shark current = bfs.front();
        bfs.pop();

        for (int i = 0; i < 4; i++) {
            int size = current.size;
            int x = current.x + moveX[i];
            int y = current.y + moveY[i];
            int eat = current.eatCnt;

            if (x < 0 || y < 0 || x >= n || y >= n || map[x][y] > size || visited[x][y]) { continue; }

            // 먹이를 찾았을때
            if (map[x][y] != 0 && size > map[x][y]) { 
                map[x][y] = 0; 
                ++eat;
                if (eat == size) {
                    eat = 0;
                    size++;
                }

                for (int i = 0; i < n; i++) {
                    for (int j = 0; j < n; j++) { visited[i][j] = false; }
                }
                while (!bfs.empty()) { bfs.pop(); }

                bfs.push({size, x, y, current.moveCnt + 1, eat});
                visited[x][y] = true;
                
                result = current.moveCnt + 1;
                traking[x][y] = current.moveCnt + 1;

                cout << "\n";
                for (int i = 0; i < n; i++) {
                    for (int j = 0; j < n; j++) {
                        cout.width(3);
                        if (i == x && j == y) {
                            cout << "S";
                        } else {
                            cout << map[i][j];
                        }
                    }
                    cout << "\n";
                }

                cout << "secno: " << secno++ << " x: " << x << " y: " << y << " eat: " << eat << " size: " << size << " move: " << current.moveCnt + 1 << "\n";
                
                break;
            }

            bfs.push({size, x, y, current.moveCnt + 1, eat});
            visited[x][y] = true;
            traking[x][y] = current.moveCnt + 1;
        }
    }
    
    cout << result << "\n";

    return 0;
}