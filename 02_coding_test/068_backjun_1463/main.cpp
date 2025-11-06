#include <iostream>
#include <vector>
#include <cstdint>

using namespace std;

bool three(int& num, int& cnt, const vector<uint32_t>& mins);
bool two(int& num, int& cnt, const vector<uint32_t>& mins);

// 3으로만 나눠지는거
bool three(int& num, int& cnt, const vector<uint32_t>& mins) {
    if (num % 3 == 0) {

        int tempCnt2 = cnt;
        int tempNum2 = num;
        two(tempNum2, tempCnt2, mins);

        num /= 3;
        if(num != 1) { 
            cnt = mins[num - 1]; 
            num = 1;
        }
        cnt++;

        if (tempCnt2 < cnt) { cnt = tempCnt2; }

        return true;
    } else if (num % 3 == 1) {
        // NUM = 1 박기 전에 2로도 풀어봐야함!
        int tempCnt = cnt;
        int tempNum = num;
        two(tempNum, tempCnt, mins);

        num--;
        cnt++;
        cnt += mins[num - 1];

        if (tempCnt < cnt) { cnt = tempCnt; }

        num = 1;
        return true;
    }
    return false;
}

// 2로만 나눠지는거
bool two(int& num, int& cnt, const vector<uint32_t>& mins) {
    if (num % 2 == 0) {
        num /= 2;
        if(num != 1) { 
            cnt = mins[num - 1]; 
            num = 1;
        }
        cnt++;
        return false;
    } else if (num % 2 == 1) {
        num--;
        cnt++;
        cnt += mins[num - 1];
        num = 1;
        return false;
    }
    return true;
}

int main(int argc, char const *argv[]) {
    cin.tie(0);
    cout.tie(0);
    ios_base::sync_with_stdio(0);

    uint32_t N;
    cin >> N;
    
    vector<uint32_t> mins(N, -1);

    for(int i = 0; i < N; i++) {
        int num = i + 1, cnt = 0;
        bool check = true;
        while ( num != 1 ) {
            if (check == true) {
                check = three(num, cnt, mins);
                if(num == 1) {break;}
            } else {
                check = two(num, cnt, mins);
            }
        }
        mins[i] = cnt;
        check = true;
    }

    cout << mins[N - 1] <<"\n";

    // for(auto& m : mins) {
    //     cout << m << " ";
    // }
    // cout << "\n";
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