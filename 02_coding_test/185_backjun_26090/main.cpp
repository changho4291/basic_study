#include <iostream>
#include <vector>

using namespace std;

int main(int argc, char const *argv[]) {
    cin.tie(0); cout.tie(0);
    ios_base::sync_with_stdio(0);

    // 에라토스 테네스의 체를 활용하여 소수를 미리 모두 필터
    vector<bool> prime(1000001, true);
    prime[0] = prime[1] = false;
    
    for (int i = 2; i * i < 1000001; i++) {
        if ( !prime[i] ) { continue; }
        for (int j = i * i; j < 1000001; j += i) { prime[j] = false; }
    }

    // 배열을 옆으로 한칸씩 이동 하면서 문제를 해결
    int n, cnt = 0;
    cin >> n;
    vector<int> nums(n);

    for (int& i : nums) { cin >> i; }

    for (int i = 2; i <= n; i++) {
        if (!prime[i]) { continue; }
        
        int k = 0;
        while ((k + i) <= n) {
            int sum = 0;
            for (int j = k; j < (k + i); j++) {
                sum += nums[j];
            }
            k++;

            if (prime[sum]) { cnt++; }
        }
    }

    cout << cnt << "\n";

    return 0;
}