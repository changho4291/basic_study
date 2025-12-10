#include <iostream>

using namespace std;

int n, m;
bool isUsed[10] = {false, };
int arr[10] = {0, };

void backTrac(int idx, int pre);

int main(int argc, char const *argv[]) {
    cin >> n >> m;

    backTrac(0, 1);
}

void backTrac(int idx, int pre) {
    if(idx == m) {
        for(int i = 0; i < m; i++) {
            cout << arr[i] << ' ';
        }
        cout << '\n';
        return;
    }

    for (int i = pre; i <= n; i++) {
        if(!isUsed[i]) {
            isUsed[i] = true;
            arr[idx] = i;
            backTrac(idx + 1, i + 1);
            isUsed[i] = false;
        }
    }
}