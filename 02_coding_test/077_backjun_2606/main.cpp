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

    while(!dfs.empty()) {
        int current = dfs.top();
        dfs.pop();
    }
    
    
    return 0;
}