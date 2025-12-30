#include <iostream>

using namespace std;

int main(int argc, char const *argv[]) {
    cin.tie(0);
    cout.tie(0);
    ios_base::sync_with_stdio(0);
    
    int a;
    while (1) {
        cin >> a;
        if (a == 0) { break; }

        int b = 0;
        for (int i = 1; i <= (2 * a); i++) {
            int c;
            cin >> c; 
            if (i % 2 != 0) {
                if (b == 0) { b += c; }
                else { b *= c; }
            } else {
                b -= c;
            }
        }
        cout << b << "\n";
    }

    return 0;
}