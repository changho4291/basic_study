#include <iostream>
#include <vector>

using namespace std;

int n, m;
int arr[10] = {0,};
bool isUsed[10] = {false, };

void backTrac(int idx);

int main(int argc, char const *argv[]) {
    cin.tie(0);
    cout.tie(0);
    ios_base::sync_with_stdio(0);

    cin >> n >> m;
    
    backTrac(0);

    return 0;
}

void backTrac(int idx) {
    if (idx == m) {
        for (int i = 0; i < m; i++) {
            cout << arr[i] << " ";
        }
        cout << "\n";
        return;
    }

    for (int i = 1; i <= n; i++) {
        arr[idx] = i;
        backTrac(idx + 1);
    }
}
