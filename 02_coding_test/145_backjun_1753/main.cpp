#include <iostream>
#include <vector>
#include <cstdint>
#include <queue>

using namespace std;

const uint64_t INF = (1ull << 62);

int main(int argc, char const *argv[]) {
    cin.tie(0);
    cout.tie(0);
    ios_base::sync_with_stdio(0);

    int n, e, k;
    cin >> n >> e >> k;

    vector<uint64_t> path_value(n, INF);
    vector<vector<pair<int, uint32_t>>> graph(n);
    priority_queue<pair<uint64_t, int>, vector<pair<uint64_t, int>>, greater<pair<uint64_t, int>>> pque;

    int u, v;
    uint32_t w;
    for(int i = 0; i < e; i++) {
        cin >> u >> v >> w;
        graph[u - 1].push_back({v - 1, w});
    }

    path_value[k - 1] = 0;
    pque.push({0, k - 1});

    while (!pque.empty()) {
        auto [f, s] = pque.top();
        pque.pop();
        if (f != path_value[s]) { continue; }

        for(auto& [no, val] : graph[s]) {
            uint64_t tmp = f + val;
            if (path_value[no] > tmp) { 
                path_value[no] = tmp; 
                pque.push({tmp, no});
            }
        }
    }
    
    for(auto& p : path_value) {
        if (p == INF) {
            cout << "INF ";
        } else {
            cout << p << " ";
        }
    }
    cout << "\n";

    return 0;
}
