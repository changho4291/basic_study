#include <iostream>
#include <stack>
#include <algorithm>

using namespace std;

int main(int argc, char const *argv[]) {
    cin.tie(0); cout.tie(0);
    ios_base::sync_with_stdio(0);

    string s;
    cin >> s;
    
    stack<char> commands;
    stack<int> scores;
    for(char& c : s) { commands.push(c); }

    while (!commands.empty()) {
        char c = commands.top();
        commands.pop();

        if (c == 'x') {
            scores.push(0);
        } else if (c == 'g') {
            if (scores.empty()) { 
                cout << "-1\n";
                return 0; 
            }

            int score = scores.top();
            scores.pop();
            scores.push(++score);
        } else {
            int score[2];
            for (int i = 0; i < 2; i++) {
                if (scores.empty()) { 
                    cout << "-1\n";
                    return 0; 
                }

                score[i] = scores.top();
                scores.pop();
            }

            scores.push(min(score[0], score[1]));
        }
    }
    
    if (scores.size() != 1)  { cout << "-1\n"; }
    else { cout << scores.top() << "\n"; }

    return 0;
}