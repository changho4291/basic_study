#include <iostream>
#include <vector>
#include <queue>

using namespace std;

 typedef struct {
    int x;
    int y;
    int z;
    int depth;
 } Data;

int main () {
    cin.tie(0);
    cout.tie(0);
    ios_base::sync_with_stdio(0);

    int moveX[] = {0, 1, 0, -1, 0, 0};
    int moveY[] = {1, 0, -1, 0, 0, 0};
    int moveZ[] = {0, 0, 0, 0, 1, -1};

    int l, r, c;    // 높이, 행, 열

    while (1) {
        cin >> l >> r >> c;
        if ( !l && !r && !c) {break;}

        vector<vector<vector<char>>> building(l, vector<vector<char>>(r, vector<char>(c)));
        vector<vector<vector<bool>>> isVisited(l, vector<vector<bool>>(r, vector<bool>(c, false)));
        queue<Data> bfs;

        for (int i = 0; i < l; i++) {
            for (int j = 0; j < r; j++) {
                cin.get();
                for (int z = 0; z < c; z++) {
                    building[i][j][z] = cin.get();
                    if (building[i][j][z] == 'S') {
                        bfs.push({i, j, z, 0});
                        isVisited[i][j][z] = true;
                    }
                }
            }
            cin.get();
        }

        bool isEscape = false;

        while (!bfs.empty()) {
            auto current = bfs.front();
            bfs.pop();
            
            for (int i = 0; i < 6; i++) {
                int x = current.x + moveX[i];
                int y = current.y + moveY[i];
                int z = current.z + moveZ[i];

                if (x < 0 || y < 0 || z < 0 || x >= l || y >= r || z >= c) { continue; }
                if (building[x][y][z] == '#' || isVisited[x][y][z]) { continue; }

                if (building[x][y][z] == 'E') { 
                    isEscape = true;
                    break;
                }
                bfs.push({x, y, z, current.depth + 1});
                isVisited[x][y][z] = true;
            }

            if (isEscape) {
                cout << "Escaped in " << current.depth + 1 << " minute(s).\n";
                break;
            }
        }

        if (!isEscape) { cout << "Trapped!\n"; }
    }
    
    return 0;
}