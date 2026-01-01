#include <iostream>

using namespace std;

int main(int argc, char const *argv[]) {
    cin.tie(0);
    cout.tie(0);
    
    int a;
    cin >> a;

    switch (a / 10) {
    case 10:
    case 9:
        cout << "A\n";
        break;

    case 8:
        cout << "B\n";
        break;

    case 7:
        cout << "C\n";
        break;

    case 6:
        cout << "D\n";
        break;
    
    default:
        cout << "F\n";
        break;
    }
        
    return 0;
}