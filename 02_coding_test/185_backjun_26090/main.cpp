#include <iostream>
#include <vector>

using namespace std;

vector<bool> prime(1000001, true);

int n, m, cnt = 0;
vector<int> nums(501);
int arr[501] = {0, };
bool isUsied[501] = {false, };

void backTracking(int idx, int pre);

int main(int argc, char const *argv[]) {
    cin.tie(0); cout.tie(0);
    ios_base::sync_with_stdio(0);

    // 에라토스 테네스의 체를 활용하여 소수를 미리 모두 필터
    prime[0] = prime[1] = false;

    for (int i = 2; i * i < 1000001; i++) {
        if ( !prime[i] ) { continue; }
        for (int j = i * i; j < 1000001; j += i) { prime[j] = false; }
    }

    // 백트래킹을 활용한 완전한 수열 찾기
    cin >> n;
    for(int i = 1; i <= n; i++) { cin >> nums[i]; }

    for (int i = 2; i < n; i++) {
        if (prime[i]) {
            m = i;
            backTracking(0, 1);
        }
    }

    cout << cnt << "\n";

    return 0;
}

void backTracking(int idx, int pre) {
    if (idx == m) {
        int tmp = 0;
        for (int i = 0; i < m; i++) {   
            tmp += arr[i];
            cout << arr[i] << ' ';
        }
        if (prime[tmp]) { 
            cout << " [prime]";
            cnt++;
        }
        cout << '\n';
        return;
    }

    for (int i = pre; i <= n; i++) {
        if (!isUsied[i]) {
            isUsied[i] = true;
            arr[idx] = nums[i];
            backTracking(idx + 1, i + 1);
            isUsied[i] = false;
        }
    }
}