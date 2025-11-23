#include <iostream>
#include <stack>
#include <queue>
#include <map>

using namespace std;

int main(int argc, char const *argv[]) {
    cin.tie(0);
    cout.tie(0);
    ios_base::sync_with_stdio(0);

    string expression;
    string result = "";
    cin >> expression;
    result.reserve(expression.size());

    stack<string> num;
    stack<char> oper;

    stack<string> num2;
    queue<char> oper2;

    map<char, int> operTier = {
        {'+', 1}, {'-', 1}, {'*', 2}, {'/', 2}, {'(', 4}, {')', 0}
    };

    for (auto& c : expression) {
        if (c < 'A' || c > 'Z') {
            int currentTier = operTier[c];
            int flag = false;

            while (!oper.empty()) {
                if(operTier[oper.top()] < currentTier) { break; }
                if(currentTier != 0 && oper.top() == '(') { break; }
                if(currentTier == 0 && oper.top() == '(') {
                    oper.pop(); break;
                }

                num2.push(num.top()); num.pop();
                num2.push(num.top()); num.pop();
                oper2.push(oper.top()); oper.pop();
                flag = true;

                result += num2.top(); num2.pop();
                result += num2.top(); num2.pop();
                result += oper2.front(); oper2.pop();
                num.push(result);
                result = "";
            }
            
            if(currentTier != 0) { oper.push(c); }

        } else {
            string temp = "";
            temp = c;
            num.push(temp);
        }
    }

    while(!num.empty()) {
        num2.push(num.top()); num.pop();
    }

    while (!oper.empty()) {
        oper2.push(oper.top()); oper.pop();
    }

    while (!num2.empty()) {
        result += num2.top(); num2.pop();
    }

    while (!oper2.empty()) {
        result += oper2.front(); oper2.pop();
    }

    cout << result << "\n";

    return 0;
} 