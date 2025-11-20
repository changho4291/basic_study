#include <iostream>
#include <vector>
#include <queue>
#include <tuple>

using namespace std;

int main(int argc, char const *argv[]) {
    cin.tie(0);
    cout.tie(0);
    ios_base::sync_with_stdio(0);

    int a, b, c;
    cin >> a >> b >> c;
    vector<vector<vector<int>>> tomato(c, vector<vector<int>>(b, vector<int>(a)));
    
    
    for (int i = 0; i < c; i++) {
        for (int j = 0; j < b; j++) {
            for (int k = 0; k < a; k++) {
                cin >> tomato[i][j][k];
            }
        }
    }

    return 0;
} 