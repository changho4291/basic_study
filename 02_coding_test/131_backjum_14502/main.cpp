#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

typedef struct {
    int n, m;
    int wallNum = 0;
    vector<vector<int>> lab;        // 연구소 지도
    vector<pair<int, int>> virus;   // 바이러스 위치
} Data;

int moveX[] = {0, 1, 0, -1};
int moveY[] = {1, 0, -1, 0};
int result = 0;

void solution(Data& data, int idx, int start);

int main(int argc, char const *argv[]) {
    cin.tie(0);
    cout.tie(0);
    ios::sync_with_stdio(0);

    int n, m;
    cin >> n >> m;

    Data data = {
        .n = n,
        .m = m,
        .lab = vector<vector<int>>(n, vector<int>(m)),
    };

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            cin >> data.lab[i][j];
            if (data.lab[i][j] == 2) { data.virus.push_back({i, j}); }
            if (data.lab[i][j] == 1) { data.wallNum++; }
        }
    }

    solution(data, 0, 0);
    cout << result << "\n";
    return 0;
}

void solution(Data& data, int idx, int start) {
    if (idx == 3) {
        int safe = data.n * data.m;
        int virusCnt = 0;
        queue<pair<int, int>> virus;
        vector<vector<bool>> isVisited(data.n, vector<bool>(data.m, false));

        for(auto& p : data.virus) { virus.push(p); virusCnt++; }

        while (!virus.empty()) {
            auto current = virus.front();
            virus.pop();

            for (int i = 0; i < 4; i++) {
                int x = current.first + moveX[i];
                int y = current.second + moveY[i];

                if (x < 0 || y < 0 || x >= data.n || y >= data.m 
                    || isVisited[x][y] || data.lab[x][y] != 0) { continue; }

                virusCnt++;
                isVisited[x][y] = true;
                virus.push({x, y});
            }
        }

        safe = safe - (virusCnt + data.wallNum + 3);
        result = max(result, safe);

        return;
    }
    
    for(int i = start; i < data.n * data.m; i++) {
        int x = i / data.m;
        int y = i % data.m;

        if (data.lab[x][y] != 0) { continue; }

        data.lab[x][y] = 1;
        solution(data, idx + 1, i + 1);
        data.lab[x][y] = 0;
    }
}