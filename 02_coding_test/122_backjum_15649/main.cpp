#include <iostream>

using namespace std;

int n, m;
int arr[10] = {0, };
bool isUsied[10] = {false, };

void backTracking(int idx);

int main(int argc, char const *argv[]) {
    cin.tie(0);
    cout.tie(0);
    ios_base::sync_with_stdio(0);

    cin >> n >> m;

    backTracking(0);
    
    return 0;
}

void backTracking(int idx) {
    if (idx == m) {
        for (int i = 0; i < m; i++) {
            cout << arr[i] << ' ';
        }
        cout << "\n";
        return;
    }

    for (int i = 1; i <= n; i++) {
        if (!isUsied[i]) {
            isUsied[i] = true;
            arr[idx] = i;
            backTracking(idx + 1);
            isUsied[i] = false;
        }
    }
}