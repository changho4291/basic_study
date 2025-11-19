#include <iostream>
#include <stack>

using namespace std;

void calculator(stack<int>& nums, stack<char>& oper) {
    while (!oper.empty()) {
    char op = oper.top(); oper.pop();
    int first = nums.top(); nums.pop();
    int second = nums.top(); nums.pop();

    if (op == '+') {
        nums.push(second + first);
    } else {
        nums.push(second - first);
    }
}
}

int main(int argc, char const *argv[]) {
    cin.tie(0);
    cout.tie(0);
    ios_base::sync_with_stdio(0);

    stack<int> nums;
    stack<char> oper;
    string calculate;
    cin >> calculate;

    nums.push(0);
    string buffer = "";
    for (char& a : calculate) {
        if ('0' <= a && a <= '9') {
            buffer.push_back(a);
        } else {
            if (!buffer.empty() && buffer != "") {
                nums.push(stoi(buffer));
                buffer = "";
            }

            if (a == '+') {
                oper.push(a);
            } else {
                if (!oper.empty()) {
                    calculator(nums, oper);
                }
                oper.push(a);
            }
        }
    }
    nums.push(stoi(buffer));
    
    calculator(nums, oper);

    cout << nums.top() << "\n";

    return 0;
}