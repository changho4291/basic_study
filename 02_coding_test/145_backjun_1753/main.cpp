#include <iostream>
#include <vector>
#include <cstdint>
#include <queue>

using namespace std;

#define INF -1

typedef struct Data_ {
    int node;
    int next;
    uint32_t value;

    Data_(int node_, int next_, uint32_t value_) : node(node_), next(next_), value(value_) {}

    bool operator<(const Data_ d) const {
        return this->value > d.value;
    }
} Data;

int main(int argc, char const *argv[]) {
    cin.tie(0);
    cout.tie(0);
    ios_base::sync_with_stdio(0);

    int n, e, k;
    cin >> n >> e >> k;

    vector<uint32_t> path_value(n, INF);
    vector<vector<Data>> graph(n);
    priority_queue<Data> pque;

    int u, v;
    uint32_t w;
    for(int i = 0; i < e; i++) {
        cin >> u >> v >> w;
        graph[u - 1].push_back({u - 1, v - 1, w});
        if (u == k) {
            pque.push({u - 1, v - 1, w});
        }
    }

    path_value[k - 1] = 0;

    while (!pque.empty()) {
        auto current = pque.top();
        pque.pop();

        uint32_t tmp = path_value[current.node] + current.value;
        if (path_value[current.next] > tmp) { 
            path_value[current.next] = tmp; 
            
            for(auto& d : graph[current.next]) { pque.push(d); }
        }
    }
    
    for(auto& p : path_value) {
        if (p != -1) {
            cout << p << " ";
        } else {
            cout << "INF ";
        }
    }
    cout << "\n";

    return 0;
}
