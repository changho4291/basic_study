#include <iostream>
#include <vector>
#include <queue>

using namespace std;

int main(int argc, char const *argv[]) {
    cin.tie(0);
    cout.tie(0);
    ios_base::sync_with_stdio(0);

    vector<bool> visited(200001, false);
    queue<pair<int, int>> bfs;
    int move[3] = {1, -1};

    int n, k;
    cin >> n >> k;

    if( n == k ) {
        cout << "0\n";
        return 0;
    }
    
    bfs.push({n, 0});
    visited[n] = true;

    while (!bfs.empty()) {
        auto current = bfs.front();
        bfs.pop();
        move[2] = current.first;
        
        for (int i = 0; i < 3; i++) {
            int point = current.first + move[i];

            if (point < 0 || point > 100000) { continue; }
            if (visited[point]) { continue; }

            if (point == k) {
                cout << current.second + 1 << "\n";
                return 0;
            }

            bfs.push({point, current.second + 1});
            visited[point] = true;
        }
    }

    return 0;
}