#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main(int argc, char const *argv[]) {
    cin.tie(0);
    cout.tie(0);
    ios_base::sync_with_stdio(0);

    int n;
    cin >> n;
    vector<pair<int, int>> origin(n);
    vector<int> sort_arr(n);

    int c;
    for (int i = 0; i < n; i++) {
        cin >> c;
        origin[i] = {c, i};
    }

    sort(origin.begin(), origin.end());

    int i = 0;
    for (auto& [f, s] : origin) {
        sort_arr[s] = i;
        i++;
    }

    for (auto& r : sort_arr) {
        cout << r << " ";
    }
    cout << "\n";

    return 0;
}
