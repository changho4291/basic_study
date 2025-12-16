#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

// vector<vector<int>> arr;

int n, r, c;
bool flag = false;
int sumarr[2][2] = {{0, 1}, {2, 3}};

void callback(int x, int y, int start, int size, int len);

int main(int argc, char const *argv[]) {
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);

    cin >> n >> r >> c;
    // arr.resize((int)pow(2, n), vector<int>((int)pow(2, n)));

    callback(0, 0, 0, (int)pow(2, n) * (int)pow(2, n), (int)pow(2, n));

    // cout << arr[r][c] << "\n";
    
    return 0;
}

void callback(int x, int y, int start, int size, int len) {
    if (flag) { return; }

    if (size / 4 == 1) {
        int a = ((x + 2) - r);
        int b = ((y + 2) - c);
    
        if (a > 4 || b > 4 || a <= 0 || b <= 0) { return; } 
        cout << start + sumarr[a][b] << "\n";
        flag = true;
        
        // for (int i = 0; i < 2; i++) {
        //     for (int j = 0; j < 2; j++) {
        //         if (x + i == r && y + j == c) {
        //             cout << start + sumarr[i][j] << "\n";
        //             flag = true;
        //         }
        //     }
        // }

        return;
    }

    callback(x, y, start, size / 4, len / 2);
    callback(x, y + len / 2, start + size / 4, size / 4, len / 2);
    callback(x + len / 2 , y, start + (size / 4) * 2, size / 4, len / 2);
    callback(x + len / 2 , y + len / 2, start + (size / 4) * 3, size / 4, len / 2);
}