#include <iostream>

using namespace std;

int main(int argc, char const *argv[]) {
    cin.tie(0);
    cout.tie(0);
    ios_base::sync_with_stdio(0);

    int k;
    cin >> k;

    for (int i = 0; i < k; i++) { cout << "1"; }
    for (int i = 0; i < k - 1; i++) { cout << "0"; }
    cout << "\n";
    return 0;
}