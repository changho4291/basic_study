#include <iostream>
#include <vector>
#include <queue>
#include <cmath>

using namespace std;

bool inRange(int x1, int y1, int x2, int y2, int R) {
    long long dx = x1 - x2;
    long long dy = y1 - y2;
    return dx*dx + dy*dy <= 1LL * R * R;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, r, d;
    pair<int,int> enemy;
    cin >> n >> r >> d >> enemy.first >> enemy.second;

    vector<pair<int,int>> tower(n);
    for (auto &t : tower) cin >> t.first >> t.second;

    vector<vector<int>> graph(n + 1);

    for (int i = 0; i < n; ++i) {
        if (inRange(enemy.first, enemy.second, tower[i].first, tower[i].second, r))
            graph[0].push_back(i + 1);
    }

    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            if (inRange(tower[i].first, tower[i].second,
                        tower[j].first, tower[j].second, r)) {
                graph[i + 1].push_back(j + 1);
                graph[j + 1].push_back(i + 1);
            }
        }
    }

    // BFS
    vector<int> dist(n + 1, -1);
    queue<int> bfs;
    dist[0] = 0;
    bfs.push(0);

    double result = 0.0;

    while (!bfs.empty()) {
        int cur = bfs.front(); 
        bfs.pop();

        for (int nxt : graph[cur]) {
            if (dist[nxt] != -1) continue;
            dist[nxt] = dist[cur] + 1;
            bfs.push(nxt);

            result += d / pow(2.0, dist[nxt] - 1);
        }
    }

    cout << fixed;
    cout.precision(5);
    cout << result << "\n";
}

// typedef struct {
//     int x;
//     int y;
//     int depth;
// } node;

// bool inRange(int x1, int y1, int x2, int y2, int R) {
//     long long dx = x1 - x2;
//     long long dy = y1 - y2;
//     return dx*dx + dy*dy <= 1LL*R*R;
// }


// int main(int argc, char const *argv[]) {
//     cin.tie(0);
//     cout.tie(0);
//     ios_base::sync_with_stdio(0);

//     int moveX[4] = {0, 1, 0, -1};
//     int moveY[4] = {1, 0, -1, 0};

//     int n, r, d;
//     pair<int, int> enumy;
//     cin >> n >> r >> d >> enumy.first >> enumy.second;

//     vector<pair<int, int>> towers(n);

//     for (auto& t : towers) {
//         cin >> t.first >> t.second;
//     }

//     queue<node> bfs;

//     // cout << fixed;
//     // cout.precision(5);
//     // cout << result << "\n";

//     return 0;
// }
