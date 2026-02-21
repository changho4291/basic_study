#include <iostream>
#include <vector>

using namespace std;

int main(int argc, char const *argv[]) {
    cin.tie(0); cout.tie(0);
    ios_base::sync_with_stdio(0);

    int cx, cy;
    cin >> cx >> cy;
    vector<vector<char>> church(cx, vector<char>(cy));

    cin.get();
    for (int i = 0; i < cx; i++) {
        for (int j = 0; j < cy; j++) {
            church[i][j] = cin.get();
        }
        cin.get();
    }

    int shakeX[] = {-1, -1, -1, 0, 1, 1, 1, 0};
    int shakeY[] = {-1, 0, 1, 1, 1, 0, -1, -1};

    // 철수가 앉을 자리 탐색 (가장 많이 악수 가능한 위치로)
    int oMax = 0;
    int chulsuX = -1, chulsuY = -1;
    for (int i = 0; i < cx; i++) {
        for (int j = 0; j < cy; j++) {
            if (church[i][j] != '.') { continue; }

            int oCnt = 0;
            for (int k = 0; k < 8; k++) {
                int x = i + shakeX[k];
                int y = j + shakeY[k];

                if (x < 0 || y < 0 || x >= cx || y >= cy || church[x][y] == '.') { continue; }
                // if (x < 0 || y < 0 || x >= cx || y >= cy ) { continue; }
                // cout << "x: " << x << " y: " << y <<" church[x][y]: " << church[x][y] << "\n";
                // if (church[x][y] == '.') { continue; }

                oCnt++;
            }
            
            if (oMax < oCnt) { 
                // cout << "i: " << i << " j: " << j << "\n";
                oMax = oCnt; 
                chulsuX = i;
                chulsuY = j;
            }
        }
    }

    if (chulsuX > -1) { church[chulsuX][chulsuY] = 'o'; }

    // 악수 횟수 카운트
    int cnt = 0;
    for (int i = 0; i < cx; i++) {
        for (int j = 0; j < cy; j++) {
            if (church[i][j] != 'o') { continue; }

            int oCnt = 0;
            for (int k = 0; k < 8; k++) {
                int x = i + shakeX[k];
                int y = j + shakeY[k];

                if (x < 0 || y < 0 || x >= cx || y >= cy || church[x][y] == '.') { continue; }

                cnt++;
            }
            church[i][j] = '.'; // 악수를 한바퀴 싹 해버린 사람은 더이상 악수 할 필요 없음
        }
    }

    cout << cnt << "\n";

    return 0;
}