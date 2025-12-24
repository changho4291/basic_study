#include <iostream>

using namespace std;

int main(int argc, char const *argv[]) {
    cin.tie(0);
    cout.tie(0);
    ios_base::sync_with_stdio(0);

    int n;
    string s;
    cin >> n >> s;

    char java[] = {'J', 'A', 'V'};

    for (int i = 0; i < 3; i++) {
        for (char& c : s) {
            if(c == java[i]) { 
                n--;
                c = -1; 
            }
        }
    }

    
    for (char& c : s) {
        if(c != -1) { cout << c; }
    }
    if (n == 0) { cout << "nojava"; }
    cout << "\n";
    return 0;
}
