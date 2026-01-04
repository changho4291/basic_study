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
    vector<pair<int, int>> back_table(n, {-1, -1});
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
    back_table[a - 1] = {a - 1, 1};

    while (!pque.empty()) {
        auto [f, s] = pque.top();
        pque.pop();

        if (f != min_table[s]) { continue; }
        
        for (auto& [i, v] : graph[s]) {
            uint64_t temp = (v != INF) ? f + v : INF;
            if (temp < min_table[i]) {
                back_table[i] = {s, back_table[s].second + 1};
                min_table[i] = temp;
                pque.push({temp, i});
            }
        }
    }

    cout << min_table[b - 1] << "\n";
    cout << back_table[b - 1].second << "\n";

    vector<int> result;

    int id = b - 1;
    result.push_back(id + 1);
    while (1) {
        id = back_table[id].first;
        result.push_back(id + 1);
        if (id + 1 == a) {break;}
    }

    for(int i = 1; i <= result.size(); i++) {
        cout << result[result.size() - i] << " ";
    }
    cout << "\n";

    return 0;
}
