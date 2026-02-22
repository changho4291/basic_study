#include <iostream>
#include <vector>

using namespace std;

int main(int argc, char const *argv[]) {
    cin.tie(0); cout.tie(0);
    ios_base::sync_with_stdio(0);

    int t;
    cin >> t;

    for (int i = 0; i < t; i++) {
        int n, max = 0, maxNum = 0, cnt = 0;
        cin >> n;

        int a;
        for (int j = 1; j <= n; j++) {
            cin >> a;
            if (a > max) {
                max = a;
                maxNum = j;
            } else if (a == max) {
                maxNum = -1;
            }

            cnt += a;
        }

        cnt -= max;

        if (maxNum == -1) { cout << "no winner\n"; }
        else if (max > cnt) { cout <<  "majority winner " << maxNum << "\n"; }
        else { cout << "minority winner " << maxNum << "\n"; }
    }

    return 0;
}