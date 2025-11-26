#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

int main(int argc, char const *argv[]) {
    cin.tie(0);
    cout.tie(0);
    ios_base::sync_with_stdio(0);
    
    int n;
    cin >> n;

    // 2차원 배열 열은 각 순서별 노드를, 1행은 부모, 2행부터는 자식
    vector<vector<int>> tree(n, vector<int>(1, -1));
    vector<bool> visited(n, false);
    queue<int> bfs;

    for(int i = 0; i < n; i++) {
        int temp;
        cin >> temp;
        if (temp != -1) {
            tree[temp].push_back(i);
            tree[i][0] = temp;
        } 
    }

    // 삭제 로직
    int eraseNode;
    cin >> eraseNode;

    int parrent = tree[eraseNode][0];

    if (parrent != -1) {
        tree[parrent].erase(remove(tree[parrent].begin(), tree[parrent].end(), eraseNode)); // 부모에게서 자식 연결 삭제
    } 

    // 순회 로직
    int cntReef = 0;
    for(int i = 0; i < tree.size(); i++) {
        if (tree[i][0] == -1 && i != eraseNode) {
            bfs.push(i);
            visited[i] = true;
            if (tree[i].size() == 1) { cntReef++; } // 최상위 노드가 1개일 경우도 확인 필요
        }
    }

    while (!bfs.empty()) {
        int current = bfs.front();
        bfs.pop();

        for(int i = 1; i < tree[current].size(); i++) {
            int node =  tree[current][i];
            if(visited[node]) { continue; }
            if (tree[node].size() == 1) { cntReef++; } // 부모만 타겟으로 보고 있는 경우는 잎노드

            bfs.push(node);
            visited[node] = true;
        }
    }
    
    cout << cntReef << "\n";

    return 0;
}
