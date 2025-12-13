// 노드를 뒤집어서 깊이우선 탐색을 하고 서로 가장 가까운 위치에 있는 부모를 답으로
// 최적?화 버전

#include <iostream>
#include <vector>
#include <stack>

using namespace std;

void solution();

int main(int argc, char const *argv[]) {
    cin.tie(0);
    cout.tie(0);
    ios::sync_with_stdio(0);

    int t;
    cin >> t;

    for (int i = 0; i < t; i++) {
        solution();
    }
    return 0;
}

void solution() {
    int n, a, b;
    cin >> n;
    
    vector<int> nodes(n+1, 0);
    vector<bool> isVisited(n+1, false);

    stack<int> dfs;

    for (int i = 1; i < n; i++ ) {
        cin >> a >> b;
        nodes[b] = a;
    }
    cin >> a >> b;
    
    isVisited[a] = true;
    isVisited[b] = true;

    dfs.push(a);
    while (!dfs.empty()) {
        int cur = dfs.top();
        dfs.pop();

        if (nodes[cur] == 0) { continue; }
        if (isVisited[nodes[cur]]) {
            cout << nodes[cur] << "\n";
            return;
        }

        isVisited[nodes[cur]] = true;
        dfs.push(nodes[cur]);
    }

    dfs.push(b);
    while (!dfs.empty()) {
        int cur = dfs.top();
        dfs.pop();

        if (nodes[cur] == 0) { continue; }
        if (isVisited[nodes[cur]]) {
            cout << nodes[cur] << "\n";
            return;
        }

        dfs.push(nodes[cur]);
    }
}