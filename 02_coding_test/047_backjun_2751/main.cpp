#include <iostream>
// #include <set>
#include <vector>
#include <algorithm>

using namespace std;

int main(int argc, char const *argv[]) {
    cin.tie(0);
    ios_base::sync_with_stdio(0);

    int N, num;
    cin >> N;
    
    vector<int> vectorInt(N, 0);

    for (int& i : vectorInt) {
        cin >> i;
    }

    sort(begin(vectorInt), end(vectorInt));
    
    for (int& n : vectorInt) {
        cout << n << "\n";
    }

    return 0;
}