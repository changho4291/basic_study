#include <iostream>
#include <cstring>

using namespace std;

bool parseSyntax(string& syntax, int& idx) {
    if (syntax[idx] == '(') {
        if (syntax[idx] == '(') { 
            idx++; 
        } else { 
            return false; 
        }
        
        if ( !parseSyntax(syntax, idx)) { return false; }
    
        if (syntax[idx] == ')') { 
            idx++; 
        } else { 
            return false;
        }
        
        if ( !parseSyntax(syntax, idx) ) { return false; }
    }

    return true;
}

int main(int argc, char const *argv[]) {
    cin.tie(0);
    ios_base::sync_with_stdio(0);

    int num;
    string syntax;

    cin >> num;

    int idx;
    for (int i = 0; i < num; i++) {
        cin >> syntax;

        idx = 0;
        if (parseSyntax(syntax, idx)) {
            if (syntax[idx] == '\0') {
                cout << "YES\n";
                continue;
            }
        }

        cout << "NO\n";
    }

    return 0;
}