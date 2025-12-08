#include <iostream>
#include <vector>
#include <queue>
#include <tuple>

using namespace std;

int main(int argc, char const *argv[]) {
    cin.tie(0);
    cout.tie(0);
    ios_base::sync_with_stdio(0);

    int r, c;
    cin >> r >> c;

    vector<vector<char>> maze(r, vector<char>(c));
    vector<vector<bool>> visited(r, vector<bool>(c, false));
    queue<tuple<int, int, int>> j_bfs;
    queue<pair<int, int>> f_bfs;
    
    int moveX[4] = {0 , 1, 0, -1};
    int moveY[4] = {1, 0, -1, 0};

    char t; // 임시 문자
    for(int i = 0; i < r; i++) {
        for (int j = 0; j < c; j++) {
            cin >> t;
            if (t == '\n') { cin >> t; } // 개행문자가 있으면 한번 더 수신을 받는다.

            maze[i][j] = t;
            
            if (t == 'J') {
                j_bfs.push({i, j, 0});
                visited[i][j] = true;
            } else if (t == 'F') {
                f_bfs.push({i, j});
            }
        }
    }

    queue<tuple<int, int, int>> tj_bfs; // 임시 큐
    queue<pair<int,int>> tf_bfs; // 임시 큐

    while(!j_bfs.empty()) {

        // 지훈이 먼저 이동하고 그 다음은 불이 이동
        while (!j_bfs.empty()) {
            tj_bfs.push(j_bfs.front());
            j_bfs.pop();
        }

        while(!tj_bfs.empty()) {
            auto current = tj_bfs.front();
            tj_bfs.pop();

            // 불이 따라온 경우
            if (maze[get<0>(current)][get<1>(current)] == 'F') { continue; }

            // 탈출
            if (get<0>(current)== 0 || get<1>(current)== 0 || get<0>(current) == r - 1 || get<1>(current) == c - 1) {
                cout << get<2>(current) + 1 << "\n";
                return 0;
            }

            maze[get<0>(current)][get<1>(current)] = '.';
            for (int i = 0; i < 4; i++) {
                int x = get<0>(current) + moveX[i];
                int y = get<1>(current) + moveY[i];

                if (x < 0 || y < 0 || x >= r || y >= c) { continue; }
                if (maze[x][y] != '.' || visited[x][y] ) { continue; }
                    
                j_bfs.push({x, y, get<2>(current) + 1});
                visited[x][y] = true;
                maze[x][y] = 'J';
            }
        }

        while (!f_bfs.empty()) {
            tf_bfs.push(f_bfs.front());
            f_bfs.pop();
        }
        
        // 그 다음 불이 모든 큐에서 pop 하면서 추격
        while (!tf_bfs.empty()) {
            auto current = tf_bfs.front();
            tf_bfs.pop();

            for (int i = 0; i < 4; i++) {
                int x = current.first + moveX[i];
                int y = current.second + moveY[i];

                if (x < 0 || y < 0 || x >= r || y >= c) { continue; }
                if (maze[x][y] == '#' || maze[x][y] == 'F' ) { continue; }

                f_bfs.push({x, y});
                maze[x][y] = 'F';
            }
        }

    }

    // 빠져나갈 수 없는 경우 실패
    cout << "IMPOSSIBLE\n";
    return 0;
}