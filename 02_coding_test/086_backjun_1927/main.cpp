#include <iostream>
#include <queue>
#include <cstdint>

using namespace std;

int main(int argc, char const *argv[]) {
    cin.tie(0);
    cout.tie(0);
    ios_base::sync_with_stdio(0);

    int n;
    cin >> n;

    priority_queue<uint32_t, vector<uint32_t>, greater<uint32_t>> minHeap;
    uint32_t input;
    for (int i = 0; i < n; i++) {
        cin >> input;

        if (input == 0) {
            if (minHeap.empty()) {
                cout << "0\n";
            } else {
                cout << minHeap.top() << "\n";
                minHeap.pop();
            }
        } else {
            minHeap.push(input);
        }
    }

    return 0;
}