#include <iostream>
#include <queue>
#include <stack>
#include <algorithm>

using namespace std;

int main(int argc, char const *argv[]) {
    cin.tie(0);
    cout.tie(0);
    ios_base::sync_with_stdio(0);

    // N: 정점 수, M: 간선 수, V: 시작 정점
    int N, M, V;

    cin >> N >> M >> V;

    vector<int> result;
    vector<vector<int>> nodes(N + 1);
    vector<bool> visited(N + 1, false);

    stack<int> dfs;
    queue<int> bfs;

    int one, two;
    for (int i = 0; i < M; i++) {
        cin >> one >> two;
        nodes[one].push_back(two);
        nodes[two].push_back(one);
    }

    // 작은 정점부터 탐색 하도록 정렬
    for(auto& i : nodes) {
        sort(i.rbegin(), i.rend());
    }

    // DFS
    dfs.push(V);
    
    while (!dfs.empty()) {
        int current = dfs.top();
        dfs.pop();
        
        if (visited[current]) { continue; }
        visited[current] = true;
        result.push_back(current);

        for (int& i : nodes[current]) {
            dfs.push(i);
        }
    }

    for (auto& i : result) {
        cout << i << " ";
    }
    cout << "\n";

    // 초기화 및 재 정렬
    for(int i = 0; i < N + 1; i++) {
        visited[i] = false;
        reverse(nodes[i].begin(), nodes[i].end());
    }
    result.clear();

    // BFS
    bfs.push(V);
    visited[V] = true;
    result.push_back(V);
    
    while (!bfs.empty()) {
        int current = bfs.front();
        bfs.pop();
        
        for (int& i : nodes[current]) {
            if (visited[i]) { continue; }
            bfs.push(i);
            visited[i] = true;
            result.push_back(i);
        }
    }

    for (auto& i : result) {
        cout << i << " ";
    }
    cout << "\n";
    
    return 0;
}