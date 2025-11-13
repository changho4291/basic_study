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

    vector<int> result(N);
    vector<vector<int>> nodes(N);
    vector<bool> visited(N, false);

    stack<int> dfs;
    queue<int> bfs;

    int one, two;
    for (int i = 0; i < M; i++) {
        cin >> one >> two;
        nodes[one - 1].push_back(two);
    }

    // 작은 정점부터 탐색 하도록 정렬
    for(auto& i : nodes) {
        sort(i.begin(), i.end());
    }

    // DFS
    dfs.push(V);
    result[0] = V;
    visited[0] = true;

    while (!dfs.empty()) {
        int cnt = 1;
        int current = dfs.top();
        dfs.pop();

        for (int& i : nodes[current]) {
            if (visited[i - 1]) { continue; }

            dfs.push(i);
            visited[i - 1] = true;
            result[cnt] = i;
            cnt++;
        }
    }

    for (auto& i : result) {
        cout << i << " ";
    }
    cout << "\n";

    // BFS

    
    return 0;
}