#include <iostream>

using namespace std;

int main(int argc, char const *argv[]) {
    cin.tie(0);
    cout.tie(0);
    
    int a, b;
    cin >> a >> b;

    if ( a > b ) {
        cout << ">\n";
    } else if (a < b) {
        cout << "<\n";
    } else {
        cout << "==\n";
    }

    return 0;
}