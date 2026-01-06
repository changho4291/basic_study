#include <iostream>
#include <vector>
#include <queue>

using namespace std;

int main(int argc, char const *argv[]) {
    cin.tie(0);
    cout.tie(0);
    ios_base::sync_with_stdio(0);

    int a, b;
    cin >> a >> b;

    int dice[] = {1, 2, 3, 4, 5, 6};

    vector<int> table(100, 0);
    vector<bool> visited(100, false);
    queue<pair<int, int>> bfs;

    int c, d;
    for (int i = 0; i < a; i++) {
        cin >> c >> d;
        table[c - 1] = d - c;
    }

    for (int i = 0; i < b; i++) {
        cin >> c >> d;
        table[c - 1] = d - c;
    }

    bfs.push({0, 0});
    visited[0] = true;

    int result = 0;
    while (!bfs.empty()) {
        auto [f, s] = bfs.front();
        bfs.pop();

        for (int i = 1; i <= 6; i++) {
            int no = i + f;

            if (no >= 100 || visited[no]) { continue; }
            visited[no] = true;
            if (table[no] && visited[no + table[no]]) { continue; }
            if (no + table[no] == 99) {
                result = s + 1;
                break;
            }
            visited[no + table[no]] = true;
            bfs.push({no + table[no], s + 1});
        }
    }
    
    cout << result << "\n";
    return 0;
}
