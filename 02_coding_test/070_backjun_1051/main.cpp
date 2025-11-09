#include <iostream>
#include <vector>

using namespace std;

int main(int argc, char const *argv[]) {
    cin.tie(0);
    cout.tie(0);
    ios_base::sync_with_stdio(0);

    char board[50][50];
    int n, m;
    cin >> n >> m;

    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            cin >> board[i][j];
        }
    }

    int max = 1;

    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            char node = board[i][j];
            for (int k = j + 1; k < m; k++) {
                if(node == board[i][k]) {
                    if (i + (k - j) > n)  { continue; }

                    if (node == board[i + (k - j)][j]) {
                        if (node == board[i + (k - j)][k]) {
                            if (max < (k - j + 1) * (k - j + 1)) {
                                max = (k - j + 1) * (k - j + 1);
                            }
                        }
                    }
                }
            }
        }
    }

    cout << max << "\n";
    
    return 0;
}