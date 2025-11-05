#include <iostream>
#include <vector>
#include <set>
#include <algorithm>

using namespace std;

int main(int argc, char const *argv[]) {
    cin.tie(0);
    cout.tie(0);
    ios_base::sync_with_stdio(0);

    int N, M;
    cin >> N >> M;

    vector<string> deudjab(N);
    set<string> deudbojab;
    for (string& s : deudjab) {
        cin >> s;
    }

    sort(deudjab.begin(), deudjab.end());

    string temp;
    for(int i = 0; i < M; i++) {
        cin >> temp;
        if(binary_search(deudjab.begin(), deudjab.end(), temp)) {
            deudbojab.insert(temp);
        }
    }

    cout << deudbojab.size() << "\n";
    for(string s : deudbojab) {
        cout << s << "\n";
    }

    return 0;
}