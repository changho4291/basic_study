#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <cmath>

using namespace std;

int main(int argc, char const *argv[]) {
    cin.tie(0);
    cout.tie(0);
    ios_base::sync_with_stdio(0);

    int n, args = 0;
    cin >> n;
    vector<int> nums(n);
    map<int, int> mins;

    for(auto& i : nums) {
        cin >> i;
        args += i;

        // 최빈값 구하기 용도로 숫자 카운트
        if(mins.find(i) == mins.end()) {
            mins[i] = 1;
        } else {
            mins[i]++;
        }
    }

    // 정렬 (중앙값 및 범위 구하는 용도)
    sort(nums.begin(), nums.end());
    
    // 산술 평균
    args = round(float(args / n));
    cout << args << "\n";
    
    // 중앙값
    cout << nums[n/2] << "\n";

    // 최빈값을 구하기 위해 먼저 벡터 내의 중복 값을 제거
    nums.erase(unique(nums.begin(), nums.end()), nums.end());

    // 최빈값, 여러 개 있을 때에는 최빈값 중 두 번째로 작은 값을 출력한다.
    int minNum = 4001;
    int minCnt = 50001;
    for (auto& i : nums) {
        mins[i]
    }

    return 0;
}