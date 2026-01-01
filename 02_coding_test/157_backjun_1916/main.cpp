#include <iostream>
#include <vector>
#include <cstdint>
#include <queue>

const uint64_t INF = 1ull << 63;

using namespace std;

int main(int argc, char const *argv[]) {
    cin.tie(0);
    cout.tie(0);
    ios_base::sync_with_stdio(0);

    int n, m;
    cin >> n >> m;

    vector<uint64_t> min_table(n, INF);
    vector<vector<pair<int, uint64_t>>> graph(n);
    priority_queue<pair<uint64_t, int>, vector<pair<uint64_t, int>>, greater<pair<uint64_t, int>>> pque;

    int a, b;
    uint64_t c;
    for (int i = 0; i < m; i++) {
        cin >> a >> b >> c;
        graph[a - 1].push_back({b - 1, c});
    }

    cin >> a >> b;

    pque.push({0, a - 1});
    min_table[a - 1] = 0;

    while ( !pque.empty() ) {
        auto[f, s] = pque.top();
        pque.pop();

        if (f > min_table[s]) { continue;; }

        for ( auto& [no, value] : graph[s] ) {
            uint64_t tmp = (value != INF) ? f + value : INF;    // INF 오버플로우 방지
            if (min_table[no] > tmp) { 
                min_table[no] = tmp; 
                pque.push({tmp, no});
            }
        }
    }

    cout << min_table[b - 1] << "\n";

    return 0;
}
