#include <iostream>
#include <vector>
#include <stack>
#include <map>

using namespace std;

int main(int argc, char const *argv[]) {
    cin.tie(0);
    cout.tie(0);
    ios_base::sync_with_stdio(0);

    int N, cnt = 0;
    cin >> N;

    stack<int> dfs;
    vector<bool> visited(N);
    map<int, vector<int>> nodeMap;

    for (int i = 1; i <= N; i++) {
        vector<int> create;
        nodeMap.insert({i, create});
    }

    cin >> N;

    for (int i = 0; i < N; i++) {
        int temp1, temp2;
        cin >> temp1 >> temp2;
        nodeMap[temp1].push_back(temp2);
        nodeMap[temp2].push_back(temp1);
    }

    dfs.push(1);
    visited[0] = true;

    while(!dfs.empty()) {
        int current = dfs.top();
        dfs.pop();

        for(auto& i : nodeMap[current]) {
            // 이미 방문 했다면 나가리
            if (visited[i - 1]) { continue; }

            // 방문 하지 않았다면 방명록 작성 및 푸시 
            visited[i - 1] = true;
            dfs.push(i);
            cnt++;  // 감염 카운트 증가
        }
    }
    
    cout << cnt << "\n";
    return 0;
}