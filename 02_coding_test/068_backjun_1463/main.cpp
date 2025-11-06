#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main(int argc, char const *argv[]) {
    cin.tie(0);
    cout.tie(0);
    ios_base::sync_with_stdio(0);

    int N;
    cin >> N;
    
    vector<int> mins(N + 1, 0);

    for(int i = 2; i <= N; i++) {
        // 1을 뺀 경우의 수
        mins[i] = mins[i - 1] + 1;
        
        // 3으로 나눈 경우의 수
        if (i % 3 == 0) {
            mins[i] = min(mins[i], mins[i/3] + 1);
        }

        // 2로 나눈 경우의 수
        if (i % 2 == 0) {
            mins[i] = min(mins[i], mins[i/2]  + 1);
        }
    }

    cout << mins[N] <<"\n";
    return 0;
}


// 1   2   3   4   5   6   7   8   9   10  11  12  13  14  15  16  17  18  19   20  21  22  23  24  25  26

// 0   1   1   2   3   2   3   3   2   3   4   3   4   4   4   4   5   3   4    4   4   5   6   4   5   5

//     /2  /3  /2  /2  /3  -1  /2  /3  -1  -1  /3  -1          3으로 나눠서 몫이 1이 나와도..
//             /2  /2  /2  /3  /2  /3  /3  -1  /2  /3
//                 -1      /2  /2      /3  /3  /2  /2
//                                         /3      /2

// 먼저 3으로 나누고 몫이 1이면 1 빼고 이전값 참조
// 3으로 나눈 몫이 2 이상이면 2로 가서 몫이 1이면 1 빼고 이전 값 참조