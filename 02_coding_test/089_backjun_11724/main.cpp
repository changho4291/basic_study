#include <iostream>
#include <vector>
#include <queue>

using namespace std;

int main(int argc, char const *argv[]) {
    cin.tie(0);
    cout.tie(0);
    ios_base::sync_with_stdio(0);

    int n, m;
    cin >> n >> m;

    vector<vector<int>> grap(n + 1);
    vector<bool> visited(n+1, false);
    queue<int> bfs;

    // 그래프 그리기
    int node1, node2;
    for (int i = 0; i < m; i++) {
        cin >> node1 >> node2;
        
        grap[node1].push_back(node2);
        grap[node2].push_back(node1);
    }

    int count = 0;  // 요소 수 카운팅
    for(int i = 1; i <= n; i++) {
        if (!visited[i]) { count++; }
        bfs.push(i);
        visited[i] = true;

        while(!bfs.empty()) {
            int current = bfs.front();
            bfs.pop();

            for(int g : grap[current]) {
                if(visited[g]) { continue; }

                bfs.push(g);
                visited[g] = true;
            }
        }
    }

    cout << count << "\n";

    return 0;
}