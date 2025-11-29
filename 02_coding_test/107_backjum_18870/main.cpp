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
    vector<int> x(n);
    vector<int> s(n);

    for (int i = 0; i < n; i++) {
        cin >> x[i];
        s[i] = x[i];
    }

    sort(s.begin(), s.end());
    s.erase(unique(s.begin(), s.end()), s.end());

    for (auto& i : x) {
        cout << lower_bound(s.begin(), s.end(), i) - s.begin() << " ";
    }
    cout << "\n";

    return 0;
}
