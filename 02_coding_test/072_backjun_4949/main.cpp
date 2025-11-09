#include <iostream>
#include <stack>

using namespace std;

int main(int argc, char const *argv[]) {
    cin.tie(0);
    // cout.tie(0);
    ios_base::sync_with_stdio(0);

    string text;
    stack<char> syntex;
    bool checker = true;

    while (1) {
        getline(cin, text);

        if (text == ".") { break; }

        for(char& c : text) {
            if ((c == '[' || c == '{' || c == '(') && checker) {
                syntex.push(c);
            }

            if ((c == ']' || c == '}' || c == ')') && checker) {
                if (syntex.empty()) { checker = false; continue; }

                if (c == ']')  { checker = (syntex.top() == '['); }
                else if (c == '}')  { checker = (syntex.top() == '{'); }
                else if (c == ')')  { checker = (syntex.top() == '('); }


                if (checker == true) { syntex.pop(); }
            }

            if (c == '.') {
                if (checker) { 
                    if (!syntex.empty()) { cout << "no\n"; checker = true; }
                    else { cout << "yes\n"; }
                } 
                else { cout << "no\n"; checker = true; }
            }
        }
        while(!syntex.empty()) {
            syntex.pop();
        }
    }
    
    return 0;
}