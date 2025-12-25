#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdint>

using namespace std;

int main(int argc, char const *argv[]) {
    cin.tie(0);
    cout.tie(0);
    ios_base::sync_with_stdio(0);

    int n, m;
    cin >> n >> m;

    vector<int> forest(n);
    for(int& i : forest) { cin >> i; }

    sort(forest.begin(), forest.end());

    int low = 0;
    int top = forest[forest.size() - 1];
    int result = 0;

    while (low <= top) {
        int mid = (low + top) / 2;

        uint64_t temp = 0;
        for(int i = 0; i < n; i++) {
            if (forest[i] > mid) { temp += forest[i] - mid; }
        }
        if (temp >= m) {
            result = mid;
            low = mid + 1;
        } else {
            top = mid - 1;
        }
    };
    
    cout << result << "\n";

    return 0;
}
