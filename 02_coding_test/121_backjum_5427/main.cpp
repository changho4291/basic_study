#include <iostream>
#include <vector>
#include <queue>
#include <tuple>

using namespace std;

int moveX[4] = {0, 1, 0, -1};
int moveY[4] = {1, 0, -1, 0};

void solution() {
    int w, h;
    cin >> w >> h;

    vector<vector<char>> map(h, vector<char>(w));
    vector<vector<bool>> visited(h, vector<bool>(w, false));
    queue<tuple<int, int, int>> bfs;
    queue<tuple<int, int>> f_bfs;

    cin.get();
    for(int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            cin.get(map[i][j]);
            if (map[i][j] == '\n') { cin.get(map[i][j]); }

            if (map[i][j] == '@') {
                bfs.push({i, j, 0});
                visited[i][j] = true;
            }

            if (map[i][j] == '*') {
                f_bfs.push({i, j});
            }
        }
    }

    while(!bfs.empty()) {
        size_t f_size = f_bfs.size();
        while (f_size--) {
            auto current = f_bfs.front();
            f_bfs.pop();

            for (int i = 0; i < 4; i++) {
                int x = get<0>(current) + moveX[i];
                int y = get<1>(current) + moveY[i];

                if (x < 0 || y < 0 || x >= h || y >= w 
                    || map[x][y] == '*' || map[x][y] == '#') { continue; }

                f_bfs.push({x, y});
                map[x][y] = '*';
            }
        }

        size_t size = bfs.size();
        while (size--) {
            auto current = bfs.front();
            bfs.pop();

            if(get<0>(current) == 0 || get<1>(current) == 0 
                || get<0>(current) == h - 1 || get<1>(current) == w - 1) {
                cout << get<2>(current) + 1 << "\n";
                return;
            }

            for (int i = 0; i < 4; i++) {
                int x = get<0>(current) + moveX[i];
                int y = get<1>(current) + moveY[i];

                if (x < 0 || y < 0 || x >= h || y >= w 
                    || visited[x][y] || map[x][y] == '#' || map[x][y] == '*') { continue; }

                bfs.push({x, y, get<2>(current) + 1});
                visited[x][y] = true;
            }
        }
    }
    cout << "IMPOSSIBLE\n";
}

int main(int argc, char const *argv[]) {
    cin.tie(0);
    cout.tie(0);
    ios_base::sync_with_stdio(0);

    int t;
    cin >> t;

    for (int i = 0; i < t; i++) {
        solution();
    }
    
    return 0;
}