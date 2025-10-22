#include <cstdio>

const char wb[8][9] = {
    "WBWBWBWB",
    "BWBWBWBW",
    "WBWBWBWB",
    "BWBBBWBW",
    "WBWBWBWB",
    "BWBWBWBW",
    "WBWBWBWB",
    "BWBWBWBW"
};

const char bw[8][9] = {
    "WBWBWBWB",
    "BWBWBWBW",
    "WBWBWBWB",
    "BWBBBWBW",
    "WBWBWBWB",
    "BWBWBWBW",
    "WBWBWBWB",
    "BWBWBWBW"
};

int main(int argc, char const *argv[]) {
    int n, m;
    scanf("%d %d", &n, &m);

    char board[50][51];

    for (int i = 0; i < n; i++) {
        scanf("%s", board[i]);
    }
}