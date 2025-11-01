#include <iostream>
#include <deque>

using namespace std;

int main(int argc, char const *argv[]) {
    cin.tie(0);
    ios_base::sync_with_stdio(0);

    int N, n;
    deque<int> deq;
    bool flag = true;

    cin >> N;

    if (N == 1) {
        cout << "1\n";
        return 0;
    }

    for(int i = 1; i <= N; i++) {
        deq.push_back(i);
    }

    while (1) {
        deq.pop_front();
        if(deq.size() == 1) { break; }
        deq.push_back(deq.front());
        deq.pop_front();
    }

    cout << deq.front() << "\n";

    return 0;
}