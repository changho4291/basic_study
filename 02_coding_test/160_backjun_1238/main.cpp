#include <iostream>
#include <cstdint>
#include <vector>
#include <queue>

using namespace std;

const uint32_t INF = 1llu << 31;

int main(int argc, char const *argv[]) {
    cin.tie(0);
    cout.tie(0);
    ios_base::sync_with_stdio(0);

    int n, m, x;
    cin >> n >> m >> x;

    vector<vector<pair<int, uint32_t>>> graph(n);
    priority_queue<pair<uint32_t, int>, vector<pair<uint32_t, int>>, greater<pair<uint32_t, int>>> pque;

    int a, b, c;
    for (int i = 0; i < m; i++) {
        cin >> a >> b >> c;
        graph[a - 1].push_back({b - 1, c});
    }

    int result = 0;

    vector<uint32_t> xmin_table(n, INF);
    pque.push({0 , x - 1});
    xmin_table[x - 1] = 0;

    while (!pque.empty()) {
        auto[f, s] = pque.top();
        pque.pop();

        if (f > xmin_table[s]) { continue; }

        for (auto&[no, len] : graph[s])  {
            uint32_t temp = f >= INF ? INF : f + len;
            if (temp < xmin_table[no]) {
                xmin_table[no] = temp;
                pque.push({temp , no});
            }
        }
    }

    for (int i = 0; i < n; i++) {
        vector<uint32_t> min_table(n, INF);

        pque.push({0 , i});
        min_table[i] = 0;

        while (!pque.empty()) {
            auto[f, s] = pque.top();
            pque.pop();

            if (f > min_table[s]) { continue; }

            for (auto&[no, len] : graph[s])  {
                uint32_t temp = f >= INF ? INF : f + len;
                if (temp < min_table[no]) {
                    min_table[no] = temp;
                    pque.push({temp , no});
                }
            }
        }
        uint32_t temp = min_table[x - 1] + xmin_table[i];
        if (result < temp ) { result = temp; }
    }

    cout << result << "\n";

    return 0;
}
