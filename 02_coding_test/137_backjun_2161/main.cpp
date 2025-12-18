#include <iostream>
#include <queue>

using namespace std;

int main () {
    cin.tie(0);
    cout.tie(0);
    ios_base::sync_with_stdio(0);

    int n;
    cin >> n;

    queue<int> que;

    for(int i = 1; i <= n; i++) { que.push(i); }

    while (!que.empty()) {
        cout << que.front() << " ";
        que.pop();

        if (!que.empty()) {
            que.push(que.front());
            que.pop();
        }
    }
    cout << "\n";

    return 0;
}