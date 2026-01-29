#include <iostream>
#include <vector>
#include <queue>

using namespace std;

int main(int argc, char const *argv[]) {
    cin.tie(0); cout.tie(0);
    ios_base::sync_with_stdio(0);

    int n, size = 2;
    cin >> n;

    vector<vector<int>> map(n, vector<int>(n));
    vector<vector<bool>> visited(n, vector<bool>(n, false));
    queue<

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cin >> map[i][j];
            if (map[i][j] == 9) {
                
            }
        }
    }

    auto i = map;

    return 0;
}