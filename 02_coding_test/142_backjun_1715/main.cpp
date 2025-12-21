#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdint>
#include <functional>
#include <queue>

using namespace std;

int main () {
    cin.tie(0);
    cout.tie(0);
    ios_base::sync_with_stdio(0);

    int n;
    cin >> n;

    priority_queue<int, vector<int>, greater<int>> cards;
    int c;
    for(int i = 0; i < n; i++) { 
        cin >> c; 
        cards.push(c);
    }

    if (n == 1) {
        cout << "0\n";
        return 0;
    }

    uint64_t result = 0;
    while (!cards.empty()) {
        uint64_t current = cards.top();
        cards.pop();

        if (cards.empty()) { break; }

        uint64_t next = current + cards.top();
        cards.pop();

        result += next;
        cards.push(next);
    }

    cout << result << "\n";
    return 0;
}