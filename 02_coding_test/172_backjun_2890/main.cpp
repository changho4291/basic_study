#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main(int argc, char const *argv[]) {
    int r, c;
    cin >> r >> c;

    vector<pair<int, int>> scores(9);

    // 거리 계산
    for (int i = 0; i < r; i++) {
        cin.get();
        char a;
        int score = 0;
        int numSet;
        for (int j = 0; j < c; j++) {
            a = cin.get();
            if (a == '.') { score++; }
            else if (a - '0' >= 1 && a - '0' <= 9)  { 
                numSet = a - '0'; 
                scores[numSet - 1] = {score, numSet - 1};
            }
        }
    }

    sort(scores.begin(), scores.end());

    // 등수 계산
    int ranks[9];
    int max = scores[8].first;
    int rank = 1;
    ranks[scores[8].second] = rank;

    for (int i = 7; i >= 0; i-- ) {
        if (scores[i].first < max) { 
            rank++; 
            max = scores[i].first;
        }
        ranks[scores[i].second] = rank;
    }

    // 결과 출력
    for (int i = 0; i < 9; i++) {
        cout << ranks[i] << "\n";
    }

    return 0;
}