// 1. 먼저 시작에서 각 특정 정점과의 최단거리를 구함 1
// 2. 다시 특정 정점으로부터 목표까지의 최단거리를 각각 구하고 2
// 3. 2를 수행 하는 과정에서 추론된 특정 점점 사이의 최단거리를 확보한다면
// 4. 그렇다면 다익스트라를 총 3번 수행하고 합산하면 끝?

#include <iostream>
#include <vector>
#include <cstdint>
#include <queue>

const uint64_t INF = 1ull << 60;

using namespace std;

int main(int argc, char const *argv[]) {
    cin.tie(0);
    cout.tie(0);
    ios_base::sync_with_stdio(0);

    int n, e;
    cin >> n >> e;

    vector<uint64_t> min_table(n, INF);
    vector<vector<pair<int, uint64_t>>> graph(n);
    priority_queue<pair<uint64_t, int>, vector<pair<uint64_t, int>>, greater<pair<uint64_t, int>>> pque;

    int a, b;
    uint64_t c;
    for(int i = 0; i < e; i++) {
        cin >> a >> b >> c;
        graph[a - 1].push_back({b - 1, c});
        graph[b - 1].push_back({a - 1, c});
    }
    cin >> a >> b;

    pque.push({0, 0});
    min_table[0] = 0;

    while (!pque.empty()) {
        auto [f, s] = pque.top();
        pque.pop();
        if(f != min_table[s]) { continue; }

        for (auto& [no, v] : graph[s]) {
            uint64_t temp = (v != INF && f != INF) ? f + v : INF;
            if (temp < min_table[no]) {
                min_table[no] = temp;
                pque.push({temp, no});
            } 
        }
    }

    // 1에서 부터 특정 정점 사이의 거리를 구함
    uint64_t ab_len[2];
    ab_len[0] = min_table[a - 1];
    ab_len[1] = min_table[b - 1];

    // min_table 초기화
    for(int i = 0; i < n; i++) {
        min_table[i] = INF;
    }
    
    // a 정점에서 n 정점까지의 거리 구하기
    pque.push({0, a - 1});
    min_table[a - 1] = 0;

    while (!pque.empty()) {
        auto [f, s] = pque.top();
        pque.pop();
        if(f != min_table[s]) { continue; }

        for (auto& [no, v] : graph[s]) {
            uint64_t temp = (v != INF && f != INF) ? f + v : INF;
            if (temp < min_table[no]) {
                min_table[no] = temp;
                pque.push({temp, no});
            } 
        }
    }

    uint64_t a_to_n = min_table[n - 1];

    // min_table 초기화
    for(int i = 0; i < n; i++) {
        min_table[i] = INF;
    }

    // b 정점에서 n 정점까지의 거리 구하기
    pque.push({0, b - 1});
    min_table[b - 1] = 0;

    while (!pque.empty()) {
        auto [f, s] = pque.top();
        pque.pop();
        if(f != min_table[s]) { continue; }

        for (auto& [no, v] : graph[s]) {
            uint64_t temp = (v != INF && f != INF) ? f + v : INF;
            if (temp < min_table[no]) {
                min_table[no] = temp;
                pque.push({temp, no});
            } 
        }
    }

    uint64_t b_to_n = min_table[n - 1];

    // a와 b 사이의 거리
    uint64_t result1 = min_table[a - 1];
    uint64_t result2 = min_table[a - 1];

    result1 += a_to_n;
    result1 += ab_len[1];

    result2 += b_to_n;
    result2 += ab_len[0];
    
    // 정답 구하기
    if (INF > result1 || INF > result2) {
        if (result1 > result2) {
            cout << result2 << "\n";
        } else {
            cout << result1 << "\n";
        }
    } else {
        cout << "-1\n";
    }

    return 0;
}
