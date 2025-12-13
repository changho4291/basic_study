#include <iostream>
#include <vector>
#include <stack>

using namespace std;

int main(int argc, char const *argv[]) {
    cin.tie(0);
    cout.tie(0);
    ios::sync_with_stdio(0);

    int moveX[] = {0, 1, 0, -1, 1, 1, -1, -1};
    int moveY[] = {1, 0, -1 ,0, 1, -1, 1, -1};

    int n, m, cnt = 0;
    cin >> n >> m;

    vector<vector<int>> parm(n, vector<int>(m));
    vector<vector<bool>> isVsited(n, vector<bool>(m, false));
    stack<pair<int, int>> dfs;

    for (auto& v : parm) {
        for (int& i : v) {
            cin >> i;
        }
    }

    dfs.push({0, 0});

    while (!dfs.empty()) {
        auto current = dfs.top();
        dfs.pop();

        if (isVsited[current.first][current.second]) { continue; }

        stack<pair<int, int>> tmp_dfs;
        tmp_dfs.push(current);

        int overHight = 0;
        while (!tmp_dfs.empty()) {
            auto tmp = tmp_dfs.top();
            tmp_dfs.pop();

            if (isVsited[tmp.first][tmp.second]) { continue; }
            isVsited[tmp.first][tmp.second] = true;

            for (int i = 0; i < 8; i++) {
                int x = tmp.first + moveX[i];
                int y = tmp.second + moveY[i];

                if (x < 0 || y < 0 || x >= n || y >= m) { continue; }
                if (parm[tmp.first][tmp.second] < parm[x][y]) { overHight++; }
                if (isVsited[x][y]) {continue;}

                if (parm[x][y] == parm[tmp.first][tmp.second]) {
                    tmp_dfs.push({x, y});
                } else {
                    dfs.push({x, y});
                }
            }

        }
        if(overHight == 0) { cnt++; }
    }

    cout << cnt << "\n";
}