#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main(int argc, char const *argv[]) {
    cin.tie(0);
    cout.tie(0);
    ios_base::sync_with_stdio(0);

    string one, tow;
    cin >> one >> tow;

    reverse(one.begin(), one.end());
    reverse(tow.begin(), tow.end());
    
    if (stoi(one) > stoi(tow)) {
        cout << one <<"\n";
    } else {
        cout << tow <<"\n";
    }

    return 0;
}