#include <iostream>

using namespace std;

int main(int argc, char const *argv[]) {
    cin.tie(0);
    ios_base::sync_with_stdio(0);

    int nums[11];
    int N, K;

    nums[0] = 0;
    nums[1] = 1;
    for (int i = 2; i < 11; i++) {
        nums[i] = nums[i - 1] * i;
    }

    cin >> N >> K;

    if (!K || !nums[N-K]) {
        cout << "1\n";
    } else {
        cout << nums[N]/(nums[K]*nums[N-K]) << "\n";
    }
    
    return 0;
}