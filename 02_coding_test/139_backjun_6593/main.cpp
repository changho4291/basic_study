#include <iostream>
#include <vector>
#include <queue>

using namespace std;

typedef struct {
    int x;
    int y;
    int result;
} Data;


int main () {
    cin.tie(0);
    cout.tie(0);
    ios_base::sync_with_stdio(0);

    int n = 0;
    cin >> n;

    vector<vector<int>> cave(n, vector<int>(n));
    vector<vector<bool>> isVisited(n, vector<bool>(n));
    queue<Data> bfs;

    for (auto& c : cave) {
        for (int& i : c) {
            cin >> i;
        }
    }

    bfs.push({0, 0, cave[0][0]});
    isVisited[0][0] = true;

    while (!bfs.empty()) {
        
    }
    
    
    return 0;
}