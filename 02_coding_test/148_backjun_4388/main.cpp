#include <iostream>
#include <vector>

using namespace std;

int main(int argc, char const *argv[]) {
    cin.tie(0);
    cout.tie(0);
    ios_base::sync_with_stdio(0);

    string n, m;
    
    while(1) {
        cin >> n >> m;
        if (n == "0" && m == "0") { break; }

        size_t len = n.length() < m.length() ? m.length() : n.length(); // 더 큰 길이를 기준으로
        int cnt = 0;
        int carry = 0;
        for(int i = 1; i <= len; i++) {
            int a;
            int b;

            if (n.length() < i) {
                a = 0;
            } else {
                a = n[n.length() - i] - '0';
            }

            if (m.length() < i) {
                b = 0;
            } else {
                b = m[m.length() - i] - '0';
            }

            int sum = a + b + carry;
            carry = 0;
            if (sum >= 10) {
                cnt++;
                carry = 1;
            }
        }
        cout << cnt << "\n";
    }

    return 0;
}
