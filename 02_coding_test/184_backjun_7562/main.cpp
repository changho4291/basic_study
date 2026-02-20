#include <iostream>
#include <queue>
#include <vector>

using namespace std;

typedef struct {
    int x;
    int y;
    int cnt;
} Knight;

int main(int argc, char const *argv[]) {
    cin.tie(0); cout.tie(0);
    ios_base::sync_with_stdio(0);

    int moveX[] = {-2, -2, -1, 1, 2, 2, 1, -1};
    int moveY[] = {-1, 1, 2, 2, 1, -1, -2, -2};

    int n;
    cin >> n;

    for (int i = 0; i < n; i++) {
        int a;
        cin >> a;
        
        vector<vector<int>> board(a, vector<int>(a));
        vector<vector<bool>> visited(a, vector<bool>(a, false));
        queue<Knight> bfs;

        int gx, gy;
        
        cin >> gx >> gy;
        bfs.push({gx, gy, 0});
        visited[gx][gy] = true;
        
        cin >> gx >> gy;

        while (!bfs.empty()) {
            auto [cx, cy, c_cnt] = bfs.front();
            bfs.pop();

            if (cx == gx && cy == gy) {
                cout << c_cnt << "\n";
                break;
            }

            for (int i = 0; i < 8; i++) {
                int x = cx + moveX[i];
                int y = cy + moveY[i];

                if (x < 0 || y < 0 || x >= a || y >= a || visited[x][y]) { continue; }

                bfs.push({x, y, c_cnt + 1});
                visited[x][y] = true;
            }
        }
    }

    return 0;
}