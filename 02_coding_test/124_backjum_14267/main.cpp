#include <iostream>
#include <vector>

using namespace std;

void callback(vector<vector<int>>& members, vector<int>& score, int member, int sc);

int main(int argc, char const *argv[]) {
    int n, m;
    cin >> n >> m;

    vector<vector<int>> members(n + 1);
    vector<int> score(n, 0);

    for (int i = 1; i <= n; i++) {
        int j;
        cin >> j;
        if (j == -1)  { continue; }

        members[j].push_back(i);
    }

    for (int i = 0; i < m; i++) {
        int j, k;
        cin >> j >> k;

        callback(members, score, j, k);
    }

    for (int& s : score) {
        cout << s << ' ';
    }
    cout << '\n';
    
    return 0;
}

void callback(vector<vector<int>>& members, vector<int>& score, int member, int sc) {
    score[member - 1] += sc;
    for (int& m : members[member]) {
        callback(members, score, m, sc);
    }
}