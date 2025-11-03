#include <iostream>
#include <stack>

using namespace std;

int main(int argc, char const *argv[]) {
    cin.tie(0);
    ios_base::sync_with_stdio(0);

    int N, num;
    string commend;
    stack<int> st;

    cin >> N;

    for (int i = 0; i < N; i++) {
        cin >> commend;

        if (commend == "push") {
            cin >> num;
            st.push(num);
        } else if (commend == "pop") {
            if (!st.empty()) {
                cout << st.top() << "\n";
                st.pop();
            } else {
                cout << "-1\n";
            }
        } else if (commend == "top") {
            if (!st.empty()) {
                cout << st.top() << "\n";
            } else {
                cout << "-1\n";
            }
        } else if (commend == "size") {
            cout << st.size() << "\n";
        } else if (commend == "empty") {
            cout << st.empty() << "\n";
        }
    }

    return 0;
}