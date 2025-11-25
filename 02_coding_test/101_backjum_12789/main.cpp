#include <iostream>
#include <vector>
#include <stack>

using namespace std;

int main(int argc, char const *argv[]) {

    int n, m;
    cin >> n;

    stack<int> st;
    int num, check = 1;
    for(int i = 0; i < n; i++) {
        cin >> num;
        if (num != check) {
            st.push(num);
        } else if (num == check) {
            check++;
            while (!st.empty() && check == st.top()) {
                st.pop();
                check++;
            }
        }
    }

    if(st.empty()) {
        cout << "Nice\n";
        return 0;
    }
    cout << "Sad\n";

    return 0;
}
