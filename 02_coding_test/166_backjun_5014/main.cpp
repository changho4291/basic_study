#include <iostream>
#include <vector>
#include <queue>

using namespace std;

int main(int argc, char const *argv[]) {
    cin.tie(0);
    cout.tie(0);
    ios_base::sync_with_stdio(0);

    int f, s, g, u, d;
    cin >> f >> s >> g >> u >> d;

    vector<bool> visited(f, false);
    queue<pair<int, int>> bfs;

    int up_down[2] = {u, -d};
    
    bfs.push({s - 1, 0});
    visited[s - 1] = true;

    if (s == g) {
        cout << "0\n";
        return 0;
    }

    while (!bfs.empty()) {
        auto [here, cnt] = bfs.front();
        bfs.pop();

        for (int& i : up_down) {
            int move = here + i;

            if (move < 0 || move >= f || visited[move]) { continue; }

            if (move == g - 1) {
                cout << cnt + 1 << "\n";
                return 0;
            }

            bfs.push({move, cnt + 1});
            visited[move] = true;
        }
    }
    
    cout << "use the stairs\n";

    return 0;
}
