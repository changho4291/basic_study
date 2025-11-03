#include <iostream>
#include <queue>

using namespace std;

int main(int argc, char const *argv[]) {
    cin.tie(0);
    ios_base::sync_with_stdio(0);

    int N, num;
    string commend;
    queue<int> que;

    cin >> N;

    for (int i = 0; i < N; i++) {
        cin >> commend;

        if (commend == "push") {
            cin >> num;
            que.push(num);
        } else if (commend == "pop") {
            if (!que.empty()) {
                cout << que.front() << "\n";
                que.pop();
            } else {
                cout << "-1\n";
            }
        } else if (commend == "size") {
            cout << que.size() << "\n";
        } else if (commend == "empty") {
            cout << que.empty() << "\n";
        } else if (commend == "front") {
            if (!que.empty()) {
                cout << que.front() << "\n";
            } else {
                cout << "-1\n";
            }
        } else if (commend == "back") {
            if (!que.empty()) {
                cout << que.back() << "\n";
            } else {
                cout << "-1\n";
            }
        }
    }

    return 0;
}