#include <iostream>
#include <vector>
#include <stack>

using namespace std;

int main(int argc, char const *argv[]) {
    cin.tie(0);
    cout.tie(0);
    ios_base::sync_with_stdio(0);

    int a, b;

    int moveX[] = {0, 1, 0, -1, -1, 1, -1, 1};
    int moveY[] = {1, 0, -1, 0, -1, 1, 1, -1};

    do {
        cin >> a >> b;
        if (a == 0 && b == 0) { break; }

        vector<vector<int>> map(b, vector<int>(a));
        vector<vector<bool>> visited(b, vector<bool>(a, false));
        stack<pair<int, int>> dfs;
        
        for (int i = 0; i < b; i++) {
            for (int j = 0; j < a; j++) {
                cin >> map[i][j];
                if (map[i][j] == 1) {
                    dfs.push({i, j});
                }
            }
        }

        int cnt = 0;
        while (!dfs.empty()) {
            auto current = dfs.top();   
            dfs.pop();

            if (visited[current.first][current.second]) { continue; }
            cnt++;

            stack<pair<int, int>> tdfs;
            tdfs.push(current);
            while (!tdfs.empty()) {
                auto t_current = tdfs.top();   
                tdfs.pop();

                if (visited[t_current.first][t_current.second]) { continue; }

                visited[t_current.first][t_current.second] = true;

                for (int k = 0; k < 8; k++) {
                    int x = t_current.first + moveX[k];
                    int y = t_current.second + moveY[k];

                    if (x < 0 || y < 0 || x >= b || y >= a) { continue; }
                    if (map[x][y] == 0 || visited[x][y]) { continue; }

                    tdfs.push({x, y});
                }
            }
        }

        cout << cnt << "\n";
    } while (1);
    
    return 0;
}