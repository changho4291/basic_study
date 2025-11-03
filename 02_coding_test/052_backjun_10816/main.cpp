#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main(int argc, char const *argv[]) {
    cin.tie(0);
    ios_base::sync_with_stdio(0);

    int N, M, num;
    cin >> N;

    vector<int> cards(N, 0);

    for(int i = 0; i < N; i++) {
        cin >> cards[i];
    }

    sort(cards.begin(), cards.end());

    cin >> M;

    for(int i = 0; i < M; i++) {
        cin >> num;
        
        cout << upper_bound(cards.begin(), cards.end(), num) - lower_bound(cards.begin(), cards.end(), num);
        if (i != M -1) {
            cout << " ";
        } else {
            cout << "\n";
        }
    }

    return 0;
}