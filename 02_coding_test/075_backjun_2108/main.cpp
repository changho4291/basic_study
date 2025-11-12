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
    map<int, int> maxs;

    for(auto& i : nums) {
        cin >> i;
        args += i;

        // 최빈값 구하기 용도로 숫자 카운트
        if(maxs.find(i) == maxs.end()) {
            maxs[i] = 1;
        } else {
            maxs[i]++;
        }
    }

    // 정렬 (중앙값 및 범위 구하는 용도)
    sort(nums.begin(), nums.end());
    
    // 산술 평균
    args = round((float)args / n);
    cout << args << "\n";
    
    // 중앙값
    cout << nums[n/2] << "\n";

    // 최빈값을 구하기 위해 먼저 벡터 내의 중복 값을 제거
    nums.erase(unique(nums.begin(), nums.end()), nums.end());

    // 최빈값 찾기 
    int minNum = 0;
    int maxCnt = 0;
    for (auto& i : nums) {
        if (maxCnt < maxs[i]) {
            minNum = i;
            maxCnt = maxs[i];
        }
    }

    // 여러 개 있을 때에는 최빈값 중 두 번째로 작은 값을 출력한다.
    for (int i = 0; i < nums.size(); i++) {
        if (maxCnt == maxs[nums[i]] && minNum < nums[i]) {
            minNum = nums[i];
            break;
        }
    }

    cout << minNum << "\n";

    // 범위 출력
    cout << *(nums.end() - 1) - nums[0] << "\n";

    return 0;
}