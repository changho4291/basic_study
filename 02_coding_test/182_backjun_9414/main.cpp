#include <iostream>
#include <queue>
#include <cstdint>
#include <cmath>

using namespace std;

int main(int argc, char const *argv[]) {
    cin.tie(0); cout.tie(0);
    ios_base::sync_with_stdio(0);

    int t; 
    uint64_t mony = 5 * pow(10, 6);
    cin >> t;

    for (int i = 0; i < t; i++) {
        int64_t n;
        priority_queue<int64_t> estate;

        while (true) {
            cin >> n;
            if (n == 0) { break; }
            estate.push(n);
        }
        
        int a = 1, pay = 0, check = 0;
        while (!estate.empty()) {
            int64_t price = estate.top();
            estate.pop();
            
            price = 2 * pow(price, a);
            pay += price;

            if (mony < pay) {
                cout << "Too expensive\n";
                check = 1;
                break;
            }
            a++;
        }

        if (!check) { cout << pay << "\n"; }
    }
    return 0;
}