#include <iostream>
#include <vector>
#include <queue>

using namespace std;

int main(int argc, char const *argv[]) {
    cin.tie(0);
    cout.tie(0);
    ios_base::sync_with_stdio(0);

    // n: 노드 개수, m: 거리를 알고 싶은 노드 쌍의 개수, a: 노드1, b: 노드2, c: 거리
    int n, m, a, b, c;
    cin >> n >> m;

    vector<vector<pair<int, int>>> graph(n+1);
    
    for (int i = 0; i < n-1; i++) {
        cin >> a >> b >> c;
        graph[a].push_back( {b, c} );
        graph[b].push_back( {a, c} );
    }

    for (int i = 0; i < m; i++) {
        vector<bool> visited(n, false);
        queue<pair<int, int>> bfs;

        cin >> a >> b;
        bfs.push({a, 0});
        visited[a] = true;

        int result = 0;
        while (!bfs.empty()) {
            auto current = bfs.front();
            bfs.pop();

            for(auto& g : graph[current.first]) {
                if ( visited[g.first] ) { continue; }

                if (g.first == b) { 
                    result = current.second + g.second; 
                    break;
                }
                bfs.push( {g.first, current.second + g.second});
                visited[g.first] = true;
            }
        }
        cout << result << "\n";
    }
    
    return 0;
}