#include <iostream>
#include <vector>
#include <queue>
#include <tuple>

using namespace std;

void cntNomalRGB(const char& color, tuple<int, int, int>& nomal) {
    if (color == 'R') {
        ++get<0>(nomal);
    } else if (color == 'G') {
        ++get<1>(nomal);
    } else {
        ++get<2>(nomal);
    }
}

void cntDisabledRGB(const char& color, pair<int, int>& disabled) {
    if (color == 'R' || color == 'G') {
        disabled.first++;
    } else {
        disabled.second++;
    }
}

bool compaireDisabledColor(const char& mode, const char& current) {
    if (mode == 'R' || mode == 'G') {
        if (current == 'B') { return true; }
    } else {
        if (mode != current) { return true; }
    }
    return false;
}

int main(int argc, char const *argv[]) {
    cin.tie(0);
    cout.tie(0);
    ios_base::sync_with_stdio(0);

    int n;
    cin >> n;
    
    vector<string> sector(n);
    vector<vector<bool>> visited(n, vector<bool>(n, false));
    queue<pair<int, int>> bfs;

    int moveX[4] = {0, 1, 0, -1};
    int moveY[4] = {1, 0, -1, 0};

    for(auto& s : sector) {
        cin >> s;
    }

    // R, G, B
    tuple<int, int, int> nomal(0, 0, 0);

    // R, B
    pair<int, int> disabled(0, 0);

    // 정상인
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            if (!visited[i][j]) {
                char mode = sector[i][j];
                cntNomalRGB(mode, nomal);

                bfs.push({i, j});
                visited[i][j] = true;

                while (!bfs.empty()) {
                    pair<int, int> current = bfs.front();
                    bfs.pop();

                    for (int i = 0; i < 4; i++) {
                        int x = current.first + moveX[i];
                        int y = current.second + moveY[i];

                        if (x < 0 || y < 0 || x >= n || y >= n) { continue; }
                        if (sector[x][y] != mode || visited[x][y]) { continue; }

                        bfs.push({x, y});
                        visited[x][y] = true;
                    }
                }
            }
        }
    }

    for (auto& v : visited) {
        for (auto&& b : v) { b = false; }
    }

    // 색맹
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            if (!visited[i][j]) {
                char mode = sector[i][j];
                cntDisabledRGB(mode, disabled);

                bfs.push({i, j});
                visited[i][j] = true;

                while (!bfs.empty()) {
                    pair<int, int> current = bfs.front();
                    bfs.pop();

                    for (int i = 0; i < 4; i++) {
                        int x = current.first + moveX[i];
                        int y = current.second + moveY[i];

                        if (x < 0 || y < 0 || x >= n || y >= n) { continue; }
                        if (compaireDisabledColor(mode, sector[x][y]) || visited[x][y]) { continue; }

                        bfs.push({x, y});
                        visited[x][y] = true;
                    }
                }
            }
        }
    }

    cout << get<0>(nomal) + get<1>(nomal) + get<2>(nomal) << " " << disabled.first + disabled.second << "\n";

    return 0;
} 