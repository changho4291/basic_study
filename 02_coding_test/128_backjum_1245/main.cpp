#include <iostream>
#include <vector>
#include <stack>
#include <tuple>

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
    stack<tuple<int, int, int, int>> dfs;

    for (auto& v : parm) {
        for (int& i : v) {
            cin >> i;
        }
    }

    dfs.push({0, 0, -1, 0});

    // int pre = -1;
    // int cur = 0;

    while (!dfs.empty()) {
        auto current = dfs.top();
        dfs.pop();

        if (isVsited[get<0>(current)][get<1>(current)]) { continue; }

        int next = parm[get<0>(current)][get<1>(current)];
        if (next > get<3>(current) && get<2>(current) >= get<3>(current)) { cnt++; }

        // cout << parm[current.first][current.second] << " " << pre << " " << cur << " " << next <<"\n";

        get<2>(current) = get<3>(current);
        get<3>(current) = next;

        stack<tuple<int, int, int, int>> tmp_dfs;
        tmp_dfs.push(current);

        while (!tmp_dfs.empty()) {
            auto tmp = tmp_dfs.top();
            tmp_dfs.pop();

            if (isVsited[get<0>(tmp)][get<1>(tmp)]) { continue; }
            isVsited[get<0>(tmp)][get<1>(tmp)] = true;

            for (int i = 0; i < 8; i++) {
                int x = get<0>(tmp) + moveX[i];
                int y = get<1>(tmp) + moveY[i];

                if (x < 0 || y < 0 || x >= n || y >= m || isVsited[x][y]) { continue; }

                if (parm[x][y] == parm[get<0>(tmp)][get<1>(tmp)]) {
                    tmp_dfs.push({x, y, get<2>(tmp), get<3>(tmp)});
                } else {
                    dfs.push({x, y, get<2>(tmp), get<3>(tmp)});
                }
            }
        }
    }

    cout << cnt << "\n";
}