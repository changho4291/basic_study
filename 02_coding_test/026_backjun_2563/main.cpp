#include <cstdio>

int main(int argc, char const *argv[]) {
    int n;
    int area[100][100] = {0};

    scanf("%d", &n);

    for (int i = 0; i < n; i ++) {
        int x, y;
        scanf("%d %d", &x, &y);
        
        for(int j = 0; j < 10; j++) {
            for (int k = 0; k < 10; k++) {
                area[x + j][y + k] = 1;
            }
        }
    }

    int cnt = 0;
    for (int i = 0; i < 100; i++) {
        for(int j = 0; j < 100; j++) {
            cnt += area[i][j];
        }
    }

    printf("%d", cnt);
}