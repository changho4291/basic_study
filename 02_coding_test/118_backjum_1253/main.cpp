#include <iostream>
#include <vector>
#include <cstdint>
#include <algorithm>
#include <cmath>

using namespace std;

int main(int argc, char const *argv[]) {
    cin.tie(0);
    cout.tie(0);
    ios_base::sync_with_stdio(0);

    int n;
    cin >> n;
    vector<int64_t> nums(n);
    
    for(int64_t& i : nums) {
        cin >> i;
    }

    sort(nums.begin(), nums.end());
    int result = 0;

    // 뒤에서부터 타겟을 찾는다.
    // 타겟보다 큰 수일 경우 빼서 구할 수 있는 경우
    // 타겠보다 작을 경우 더해서 구할 수 있는 경우
    // 다른 주소로 타겠과 같은 경우는 0이 있는 경우, 타겟이 0인 경우엔 3개 이상 있을 때만

    for (int i = n - 1; i >= 0; i--) {
        bool isEven = false;
        if (nums[i] % 2 == 0) { isEven = true; }

        for (int j = n - 1; nums[j] >= nums[i]/2 && j > 0; j--) {
            if (j == i) { continue; }

            if (isEven && (nums[j] == nums[i]/2)) {
                int c = 2;
                if (nums[i] == 0) { c = 3; }
                if(upper_bound(nums.begin(), nums.end(), nums[j]) - lower_bound(nums.begin(), nums.end(), nums[j]) >= c) { result++; }
                break;
            }

            int64_t k = nums[i] - nums[j] ;
            
            if (nums[j] == 0) {
                if(upper_bound(nums.begin(), nums.end(), nums[i]) - lower_bound(nums.begin(), nums.end(), nums[i]) >= 2) { 
                    result++; 
                    break;
                }
            } else if (binary_search(nums.begin(), nums.end(), k)) {
                result++; 
                break;
            }
        }
    }

    cout << result << "\n";

    return 0;
}