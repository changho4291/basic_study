#include <iostream>
#include <vector>

using namespace std;

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

    // 기본 점수 세팅
    for (int i = 0; i < m; i++) {
        int j, k;
        cin >> j >> k;

        score[j - 1] += k;
    }

    // 내리사랑 시작
    for (int i = 2; i <= n; i++) {
        for(int& m : members[i]) {
            score[m - 1] += score[i - 1];
        }
    }

    for (int& s : score) {
        cout << s << ' ';
    }
    cout << '\n';
    
    return 0;
}