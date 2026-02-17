#include <iostream>
#include <queue>
#include <vector>

using namespace std;

int main(int argc, char const *argv[]) {
    cin.tie(0); cout.tie(0);
    ios_base::sync_with_stdio(0);

    int n, a, b;
    cin >> n;
    cin >> a >> b;

    int m, r = -1;
    cin >> m;
    vector<vector<int>> family(n + 1);
    vector<bool> visited(n + 1, false);
    queue<pair<int, int>> bfs;

    bfs.push({a, 0});
    visited[a] = true;

    for (int i = 1; i <= m; i++) {
        int x, y;
        cin >> x >> y;

        family[x].push_back(y);
        family[y].push_back(x);
    }

    while (!bfs.empty()) {
        auto [cur, cnt] = bfs.front();
        bfs.pop();

        for (int& i : family[cur]) {
            if (visited[i]) { continue; }

            if ( i == b) {
                r = cnt + 1;
                break;
            }

            bfs.push({i, cnt + 1});
            visited[i] = true;
        }
    }

    cout << r << "\n";

    return 0;
}