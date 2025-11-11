#include <iostream>
#include <stack>
#include <queue>

using namespace std;

int main(int argc, char const *argv[]) {
    cin.tie(0);
    cout.tie(0);
    ios_base::sync_with_stdio(0);

    int n, temp;
    cin >> n;
    queue<int> m;
    stack<int> k;
    queue<char> result;

    for (int i = 0; i < n; i++) {
        cin >> temp;
        m.push(temp);
    }

    for (int i = 1; i <= n; i++) {
        if(!m.empty() && i == m.front()) {
            result.push('+');
            result.push('-');
            m.pop();
            while (!m.empty() && !k.empty() && m.front() == k.top()) {
                m.pop();
                k.pop();
                result.push('-');
            }
        } else {
            result.push('+');
            k.push(i);
        }
    }

    if (k.empty()) {
        while (!result.empty()){
            cout << result.front() << "\n";
            result.pop();
        }
    } else {
        cout << "NO\n";
    }

    return 0;
}