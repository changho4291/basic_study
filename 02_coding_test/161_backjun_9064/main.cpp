#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main(int argc, char const *argv[]) {
    cin.tie(0);
    cout.tie(0);
    ios_base::sync_with_stdio(0);

    int a;
    cin >> a;

    for (int i = 0; i < a; i++) {
        int result = 0;
        int num;
        cin >> num;
        
        if (num == 6174) {
            cout << "0\n";
            continue;
        }

        while (num != 6174) {
            int tmp = num;
            int nums[4];

            for (int i = 0; i < 4; i++) {
                nums[i] = tmp % 10;
                tmp /= 10;
            }
            
            sort(nums, nums + 4);

            int min = 0, max = 0;
            
            int pow_10 = 1;
            for (int i = 0; i < 4; i++) {
                max += nums[i] * pow_10;
                min += nums[3 - i] * pow_10;
                pow_10 *= 10;
            }

            num = max - min;
            result++;
        }

        cout << result << "\n";
    }

    return 0;
}
