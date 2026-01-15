#include <iostream>
#include <vector>
#include <queue>
#include <cstdint>

using namespace std;

int r, c;
char board[20][20];

int moveX[] = {0, 1, 0, -1};
int moveY[] = {1, 0, -1, 0};

int cnt = 1;
int result = 0;

typedef struct {
    int x;
    int y;
} Data;

uint32_t alphaUsed;

void backTrack(Data data);

int main(int argc, char const *argv[]) {
    cin >> r >> c;

    for (int i = 0; i < r; i ++) {
        getchar();
        for (int j = 0; j < c; j++) {
            board[i][j] = getchar();
        }
    }

    alphaUsed |= ((uint32_t)1 << (board[0][0] - 'A'));
    backTrack({0, 0});

    if (result == 0) { result = 1; }
    cout << result << "\n";

    return 0;
}

void backTrack(Data data) {
    for (int i = 0; i < 4; i++) {
        int x = data.x + moveX[i];
        int y = data.y + moveY[i];

        if (x < 0 || y < 0 || x >= r || y >= c) {continue;}
        if (alphaUsed & ((uint32_t)1 << (board[x][y] - 'A'))) {
            result = max(result, cnt);
            continue; 
        }

        cnt++;
        alphaUsed |= ((uint32_t)1 << (board[x][y] - 'A'));
        backTrack({x, y});
        cnt--;
        alphaUsed &= ~((uint32_t)1 << (board[x][y] - 'A'));
    }
}