#include <iostream>
#include <vector>

using namespace std;

int main(int argc, char const *argv[]) {
    cin.tie(0);
    cout.tie(0);
    ios_base::sync_with_stdio(0);

    int n, s, cnt = 0;
    string str;
    vector<int> iidx;
    cin >> n >> s >> str;

    for(int i = 0; i < s; i++) {
        if (str[i] == 'I') { iidx.push_back(i); }
    }
    
    if (iidx.empty()) { 
        cout << "0\n";
        return 0; 
    }

    int c = 1;
    int current = iidx[0];
    bool prev = false;
    for(int i = 1; i < iidx.size(); i++) {
        if (prev) {
            if (iidx[i] - current == 2)  { cnt++; } 
            else { prev = false; }
        } else {
            if (iidx[i] - current == 2)  {
                c++;
                if(c == n+1) { 
                    cnt++; 
                    c = 1;
                    prev = true;
                }
            } else {
                c = 1;
            }
        }
        current = iidx[i];
    }

    cout << cnt << "\n";

    return 0;
}
