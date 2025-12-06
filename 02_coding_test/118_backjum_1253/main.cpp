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

    for (int i = n - 1; i >= 0; i--) {
        // 0이 있고 타겟 숫자가 복수개인 경우
        if (binary_search(nums.begin(), nums.end(), 0)) {
            int c = 2;
            if (nums[i] == 0) { c = 3; }
            if(upper_bound(nums.begin(), nums.end(), nums[i]) - lower_bound(nums.begin(), nums.end(), nums[i]) >= c) { 
                result++; 
                continue;
            }
        }

        bool isEven = false;

        if (nums[i] % 2 == 0) { isEven = true; }

        int j = i - 1;
        while (nums[j] >= nums[i]/2 && nums[j] < nums[i] && j >= 0) {
            if (isEven && nums[j] == nums[i]/2) {
                if(upper_bound(nums.begin(), nums.end(), nums[j]) - lower_bound(nums.begin(), nums.end(), nums[j]) >= 2) { result++; };
                break;
            }

            int64_t k = nums[i] - nums[j] ;
            if (binary_search(nums.begin(), nums.end(), k)) {
                result++; 
                break;
            }
            j--;
        }
    }

    cout << result << "\n";

    return 0;
}