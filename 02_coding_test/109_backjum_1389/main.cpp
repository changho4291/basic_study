#include <iostream>
#include <vector>
#include <set>
#include <queue>

using namespace std;

int main(int argc, char const *argv[]) {
    cin.tie(0);
    cout.tie(0);
    ios_base::sync_with_stdio(0);

    int n, m;
    cin >> n >> m;

    vector<set<int>> graph(n + 1);
    
    int tmp1, tmp2;
    for (int i = 0; i < m; i++) {
        cin >> tmp1 >> tmp2;
        graph[tmp1].insert(tmp2);
        graph[tmp2].insert(tmp1);
    }

    int result = 0;
    vector<unsigned int> cavin(n+1, -1);
    for (int i = 1; i <= n; i++) {
        vector<bool> visited(n + 1, false);
        queue<pair<int, int>> bfs;

        bfs.push({i, 0});
        visited[i] = true;
        unsigned int cnt = 0;

        while (!bfs.empty()) {
            auto current = bfs.front();
            bfs.pop();

            for (auto& j : graph[current.first]) {
                if (visited[j]) { continue; }

                visited[j] = true;
                bfs.push({j, current.second + 1});
                cnt += current.second + 1;
            }
        }
        cavin[i] = cnt;

        if (cavin[result] > cavin[i]) {
            result = i;
        }
    }
    
    cout << result << "\n";

    return 0;
}
