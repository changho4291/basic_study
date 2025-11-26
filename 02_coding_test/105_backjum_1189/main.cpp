#include <iostream>
#include <vector>
#include <queue>
#include <tuple>

using namespace std;

int main(int argc, char const *argv[]) {
    cin.tie(0);
    cout.tie(0);
    ios_base::sync_with_stdio(0);

    int r, c, k;
    int cnt = 0;    

    cin >> r >> c >> k;

    int moveX[4] = {0, 1, 0, -1};
    int moveY[4] = {1, 0, -1, 0};

    vector<string> mapTable(r);
    vector<vector<bool>> visited(r, vector<bool>(c, false));
    queue<tuple<int, int, int>> bfs;

    for (auto& s : mapTable) {
        cin >> s;
    }

    bfs.push({r-1, 0, 1});
    visited[r-1][0] = true;

    while (!bfs.empty()) {
        const auto current = bfs.front();
        bfs.pop();

        for (int i = 0; i < 4; i++) {
            int x = get<0>(current) + moveX[i];
            int y = get<1>(current) + moveY[i];

            if (x < 0 || y < 0 || x >= r || y >= c) { continue; }
            if (visited[x][y] || mapTable[x][y] == 'T') { continue; }

            bfs.push({x, y, get<2>(current) + 1});
            
            if (x != r - 1 || y != c -1) { visited[x][y] = true; }

            if ( (get<2>(current) + 1) == 6) { cnt++; }
        }
    }

    cout << cnt << "\n";

    return 0;
}
