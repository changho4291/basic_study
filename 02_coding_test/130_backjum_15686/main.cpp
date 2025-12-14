#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

typedef struct {
    int m;
    vector<pair<int, int>> house;
    vector<pair<int, int>> chicken;
    vector<int> arr;
    vector<bool> isUsed;
} Data;

int chickenLen = 1000000;

void solution(Data& data, int idx, int start);

int main(int argc, char const *argv[]) {
    // cin.tie(0);
    // cout.tie(0);
    // ios::sync_with_stdio(0);

    int n, m, k;
    Data data;
    cin >> n >> m;

    data.m = m;

    for(int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cin >> k;
            if (k == 1) { data.house.push_back({i, j}); }
            else if (k == 2) { data.chicken.push_back({i, j}); }
        }
    }

    data.arr.resize(m, 0);
    data.isUsed.resize(data.chicken.size(), false);

    solution(data, 0, 0);

    cout << chickenLen << "\n";

    return 0;
}

void solution(Data& data, int idx, int start) {
    if (idx == data.m) {
        int tmpLen = 0;

        for (auto& h : data.house) {
            int minLen = 99999;
            for (auto& a : data.arr) {
                int len = abs(h.first - data.chicken[a].first) + abs(h.second - data.chicken[a].second);
                // cout << len << "\n";
                minLen = min(minLen, len);
            }
            // cout << minLen << "\n";
            // if (tmpLen < minLen) { tmpLen = minLen; }
            tmpLen += minLen;
        }
        // cout << tmpLen << "\n\n";
        chickenLen = min(chickenLen, tmpLen);
        return;
    }

    for (int i = start; i < data.chicken.size(); i++) {
        if (data.isUsed[i] == true) { continue; }
        // cout << idx << " " << i << "\n";
        data.arr[idx] = i;
        data.isUsed[i] = true;
        solution(data, idx + 1, i + 1);
        data.isUsed[i] = false;
    }
}