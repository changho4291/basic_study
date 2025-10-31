#include <iostream>
#include <vector>
#include <set>

using namespace std;

int main(int argc, char const *argv[]) {
    cin.tie(0);
    ios_base::sync_with_stdio(0);

    int N, M;
    int num;
    cin >> N;

    set<int> A;

    while(N--) {
        cin >> num;
        A.insert(num);
    }

    cin >> M;

    while(M--) {
        cin >> num;
        if(A.find(num) != A.end()) {
            cout << "1\n";
        } else {
            cout << "0\n";
        }
    }

    return 0;
}