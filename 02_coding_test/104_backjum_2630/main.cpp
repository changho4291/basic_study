#include <iostream>
#include <vector>

using namespace std;

int white = 0;
int blue = 0;

// 흰색 0, 파랑 1, 섞임 2
int paperNum(int x1, int x2, int y1, int y2, const vector<vector<int>>& paper) {
    if (x1 == x2 && y1 == y2) {
        return paper[x1][y1];
    }

    int midX = (x1 + x2) / 2;
    int midY = (y1 + y2) / 2;

    int one = paperNum(x1, midX, y1, midY, paper);
    int tow = paperNum(midX + 1, x2, y1, midY, paper);
    int three = paperNum(x1, midX, midY + 1, y2, paper);
    int four = paperNum(midX + 1, x2, midY + 1, y2, paper);

    if (one < 2 && one == tow && one == three && one == four) {
        if (one == 0) { return 0; }
        else { return 1; }
    }

    if (one == 0) { white++; }
    if (one == 1) { blue++; }
    if (tow == 0) { white++; }
    if (tow == 1) { blue++; }
    if (three == 0) { white++; }
    if (three == 1) { blue++; }
    if (four == 0) { white++; }
    if (four == 1) { blue++; }

    return 2;
}

int main(int argc, char const *argv[]) {
    cin.tie(0);
    cout.tie(0);
    ios_base::sync_with_stdio(0);
    
    int n;
    cin >> n;

    vector<vector<int>> paper(n, vector<int>(n));
    
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cin >> paper[i][j];
        }
    }

    int result = paperNum(0, n - 1, 0, n - 1, paper);

    if (result == 0) { white++; }
    if (result == 1) { blue++; }

    cout << white << "\n";
    cout << blue << "\n";

    return 0;
}
