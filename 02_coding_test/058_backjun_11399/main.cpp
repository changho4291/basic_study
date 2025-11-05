#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdint>

using namespace std;

int main(int argc, char const *argv[]) {
    cin.tie(0);
    cout.tie(0);
    ios_base::sync_with_stdio(0);

    int N;
    uint64_t minTime = 0, temp = 0;
    cin >> N;

    vector<int> P(N);

    for (int& p : P) {
        cin >> p;
    }

    sort(P.begin(), P.end());

    for (int& p : P) {
        temp += p;
        minTime += temp;
    }

    cout << minTime;

    return 0;
}
