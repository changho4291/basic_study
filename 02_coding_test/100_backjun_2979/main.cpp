#include <iostream>
#include <vector>

using namespace std;

int main(int argc, char const *argv[]) {

    int a, b ,c;
    cin >> a >> b >> c;

    vector<int> cnt(100, 0);
    
    int d, e, result = 0;
    for (int i = 0; i < 3; i++) {
        cin >> d >> e;
        for(int j = d; j < e; j++) {
            cnt[j]++;
        }
    }

    for (auto& r : cnt) {
        if (r == 1) {
            result += a;
        } else if (r == 2) {
            result += (b * 2);
        } else if (r == 3) {
            result += (c * 3);
        }
    }

    cout << result << "\n";

    return 0;
}
