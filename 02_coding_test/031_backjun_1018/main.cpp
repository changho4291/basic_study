#include <iostream>

using namespace std;

const char wb[8][9] = {
    "WBWBWBWB",
    "BWBWBWBW",
    "WBWBWBWB",
    "BWBWBWBW",
    "WBWBWBWB",
    "BWBWBWBW",
    "WBWBWBWB",
    "BWBWBWBW"
};

const char bw[8][9] = {
    "BWBWBWBW",
    "WBWBWBWB",
    "BWBWBWBW",
    "WBWBWBWB",
    "BWBWBWBW",
    "WBWBWBWB",
    "BWBWBWBW",
    "WBWBWBWB"
};

int main(int argc, char const *argv[]) {
    cin.tie(0);
    cout.tie(0);
    ios_base::sync_with_stdio(0);

    int n, m, min = 10000;
    cin >> n >> m;

    char board[50][51];

    for (int i = 0; i < n; i++) {
         for (int j = 0; j < m; j++) {
            cin >> board[i][j];
         }
    }

    for (int i = 0; i <= n - 8; i++) {
        for (int j = 0; j <= m - 8; j++) {
            int temp = 0;
            for (int k = 0; k < 8; k++) {
                for (int l = 0; l < 8; l++) {
                    char a = wb[k][l];
                    char b = board[i + k][j + l];
                    if(wb[k][l] != board[i + k][j + l]) {
                        temp++;
                    }
                }
            }
            if (min > temp) { min = temp; }
        }
    }

    for (int i = 0; i <= n - 8; i++) {
        for (int j = 0; j <= m - 8; j++) {
            int temp = 0;
            for (int k = 0; k < 8; k++) {
                for (int l = 0; l < 8; l++) {
                    if(bw[k][l] != board[i + k][j + l]) {
                        temp++;
                    }
                }
            }
            if (min > temp) { min = temp; }
        }
    }

    cout << min << "\n";
}