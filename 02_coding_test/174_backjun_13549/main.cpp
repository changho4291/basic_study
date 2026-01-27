#include <iostream>
#include <vector>
#include <queue>

using namespace std;

int main(int argc, char const *argv[]) {
    cin.tie(0); cout.tie(0);
    ios_base::sync_with_stdio(0);

    int move[3] = {0, -1, 1, };

    int n, k;
    cin >> n >> k;

    if (n == k) {
        cout << "0\n";
        return 0;
    }

    bool isVisited[100001] = {false, };
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> bfs;

    bfs.push({0, n});
    isVisited[n] = true;

    int result = -1;

    while (!bfs.empty()) {
        auto[cnt, cur] = bfs.top();
        bfs.pop();

        move[0] = cur;

        for (int i = 0; i < 3; i++) {
            int x = cur + move[i];

            if (x < 0 || x > 100000 || isVisited[x]) { continue; }

            int tmp = cnt;
            if (i != 0) { tmp++; }

            if (x == k) {
                cout << tmp << "\n";
                return 0;
            }

            bfs.push({ tmp, x });
            isVisited[x] = true;
        }
    }

    return 0;
}