#include <iostream>
#include <vector>
#include <queue>

using namespace std;

int main(int argc, char const *argv[]) {
    cin.tie(0);
    cout.tie(0);
    ios_base::sync_with_stdio(0);

    int n;
    cin >> n;
    
    vector<vector<pair<int, int>>> graph(n);
    vector<bool> visited(n, false);
    queue<pair<int, int>> bfs;

    int a, b, c;
    for(int i = 0; i < n - 1; i++) {
        cin >> a >> b >> c;
        graph[a - 1].push_back({b - 1, c});
        graph[b - 1].push_back({a - 1, c});
    }

    if (n == 2) {
        cout << c << "\n";
        return 0;
    }

    bfs.push({0, 0});
    visited[0] = true;

    pair<int, int> maxVal = {0 , 0};
    while (!bfs.empty()) {
        auto [pNo, pVal] = bfs.front();
        bfs.pop();

        for (auto& [cNo, cVal] : graph[pNo]) {
            if (visited[cNo]) { continue; }

            bfs.push({cNo, cVal + pVal});
            visited[cNo] = true;
            if (maxVal.second < cVal + pVal) { 
                maxVal.first = cNo;
                maxVal.second = cVal + pVal; 
            }
        }
    }

    for (int i = 0; i < n; i++) {
        visited[i] = false;
    }

    bfs.push({maxVal.first, 0});
    visited[maxVal.first] = true;

    int result = 0;
    while (!bfs.empty()) {
        auto [pNo, pVal] = bfs.front();
        bfs.pop();

        for (auto& [cNo, cVal] : graph[pNo]) {
            if (visited[cNo]) { continue; }
            
            bfs.push({cNo, cVal + pVal});
            visited[cNo] = true;
            if (result < cVal + pVal) { result = cVal + pVal; }
        }
    }

    cout << result << "\n";

    return 0;
}
