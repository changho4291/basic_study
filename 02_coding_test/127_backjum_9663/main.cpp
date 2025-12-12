#include <iostream>
#include <vector>

using namespace std;

typedef struct {
    vector<bool> isUsed1;
    vector<bool> isUsed2;
    vector<bool> isUsed3;
} Status;

int cnt = 0;
int n;

void back_t(Status& s, int y);

int main(int argc, char const *argv[]) {
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);

    cin >> n;

    Status s = {
        .isUsed1 = vector<bool>(n * n, false),
        .isUsed2 = vector<bool>(n * n, false),
        .isUsed3 = vector<bool>(n * n, false),
    };

    back_t(s, 0);

    cout << cnt << "\n";

    return 0;
}

void back_t(Status& s, int y) {
    if (y == n) { 
        cnt++; 
        return;
    }

    for(int x = 0; x < n; x++) {
        if(s.isUsed1[x] || s.isUsed2[x + y] || s.isUsed3[y - x + n - 1]) { continue; }

        s.isUsed1[x] = true;
        s.isUsed2[x + y] = true;
        s.isUsed3[y - x + n - 1] = true;
        back_t(s, y + 1);
        s.isUsed1[x] = false;
        s.isUsed2[x + y] = false;
        s.isUsed3[y - x + n - 1] = false;
    }
}