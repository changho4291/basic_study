#include <iostream>

using namespace std;

int main(int argc, char const *argv[]) {
    cin.tie(0);
    cout.tie(0);
    ios_base::sync_with_stdio(0);

    int a, b, c, result = 0;
    cin >> a >> b >> c;

    int left = b - a;
    int right = c - b;
    while ( left != 1 || right != 1 ) {
        int temp = b;
        if (left < right) {
            b = c - 1;
            a = temp;
        } else {
            b = a + 1; 
            c = temp;
        }

        left = b - a;
        right = c - b;

        result++;
    }
    
    cout << result << "\n";

    return 0;
}