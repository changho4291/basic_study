#include <iostream>

using namespace std;

int main(int argc, char const *argv[]) {
    cin.tie(0);
    ios_base::sync_with_stdio(0);
    
    string num;
    while(1) {
        bool flag = true;

        cin >> num;
        if (num == "0") { break; }

        for(int i = 0; i < num.length()/2; i++) {
            if (num[i] != num[num.length() - i - 1]) {
                flag = false;
                break;
            }
        }

        if (flag) { cout << "yes\n"; } 
        else { cout << "no\n"; }
    }

    return 0;
}