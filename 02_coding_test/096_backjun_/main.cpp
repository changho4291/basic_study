#include <iostream>
#include <stack>
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

    map<char, int> operTier = {
        {'+', 1}, {'-', 1}, {'*', 2}, {'/', 2}, {'(', 4}, {')', 4}
    };

    for (auto& c : expression) {
        if (c < 'A' || c > 'Z') {
            int currentTier = operTier[c];

            while(!oper.empty()) {
                if(currentTier != 4 && oper.top() == '(') { break; }
                if(currentTier == 4 && oper.top() == '(') {
                    oper.pop(); 
                }
                if(operTier[oper.top()] < currentTier) { break; }

                string temp = "";
                temp += num.top(); num.pop();
                temp += num.top(); num.pop();
                temp += oper.top(); oper.pop();
                num.push(temp);
            }
            
            if(currentTier != 4) { oper.push(c); }

        } else {
            string temp = "";
            temp = c;
            num.push(temp);
        }
    }

    while (!num.empty()) {
        result += num.top(); num.pop();
    }

    while (!oper.empty()) {
        result += oper.top(); oper.pop();
    }

    cout << result << "\n";

    return 0;
} 