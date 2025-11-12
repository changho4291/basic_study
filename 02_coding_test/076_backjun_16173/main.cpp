// 리팩토링

#include <iostream>
#include <queue>
#include <set>

using namespace std;

using xy = pair<int, int>;

int main(int argc, char const *argv[]) {
    cin.tie(0);
    cout.tie(0);
    ios_base::sync_with_stdio(0);

    int dx[2] = {1, 0}; // 아래
    int dy[2] = {0, 1}; // 오른쪽

    int board[3][3] = {0};

    int n;
    bool visited[3][3] = {0};
    queue<xy> bfs;
    cin >> n;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cin >> board[i][j];
        }
    }

    bfs.push({0, 0});
    visited[0][0] = true;

    while (!bfs.empty()) {
        xy current = bfs.front();
        bfs.pop();

        int move = board[current.first][current.second];

        //  오른쪽, 아래 이동 비교
        for(int i = 0; i < 2; i++) { 

            // 이동 좌표 설정
            int x = current.first +  dx[i] * move;
            int y = current.second + dy[i] * move; 

            // TODO 나가리 - 못 간다.
            if (x > n - 1 || y > n - 1) { continue; } 

            // 방문 이력 찾아본다. 방문 했었다면 나가리
            if (visited[x][y]) { continue; }

            // 성공
            if (x == n - 1 && y == n - 1) {
                cout << "HaruHaru\n";
                return 0;
            }

            // 안가봤으면 기록
            bfs.push({x, y});
            visited[x][y] = true;
        }
    }

    cout << "Hing\n";
    return 0;
}