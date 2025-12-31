#include <iostream>
#include <algorithm>

using namespace std;

int main(int argc, char const *argv[]) {
    cin.tie(0);
    cout.tie(0);
    ios_base::sync_with_stdio(0);
    
    string str;
    cin >> str;

    sort(str.begin(), str.end());

    for (int i = 1; i <= str.size(); i++) {
        cout << str[str.size() - i];
    }
    cout << "\n";

    return 0;
}