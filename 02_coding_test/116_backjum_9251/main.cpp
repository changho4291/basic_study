#include <iostream>
#include <deque>
#include <sstream>
#include <algorithm>

using namespace std;

int main(int argc, char const *argv[]) {
    cin.tie(0);
    cout.tie(0);
    ios_base::sync_with_stdio(0);

    int t, n;
    string command;
    string arr;
    cin >> t;

    for (int i = 0; i < t; i++) {
        cin >> command >> n >> arr;
        
        // 배열 파싱
        deque<string> nums(n);
        string num;
        
        arr.erase(0, 1);
        arr.erase(arr.end() - 1);
        stringstream sstream(arr);
        
        for (int i = 0; i < n; i++) {
            getline(sstream, num, ',');
            nums[i] = num;
        }

        // 커맨드 실행
        int cntR = 0;
        bool rFlag = false;
        bool eFlag = false;
        for(char& c : command) {
            if (c == 'R') {
                cntR++;
                rFlag ^= true;
            } else {
                if (nums.empty()) {
                    cout << "error\n";
                    eFlag = true;
                    break;
                }

                if(!rFlag) {
                    nums.pop_front();
                } else {
                    nums.pop_back();
                }
            }
        }

        if (eFlag) { continue; }
        
        // 출력
        cout << "[";
        if (cntR % 2 != 0) {
            for(int i = nums.size()-1; i >= 0; i--) {
                cout << nums[i];
                if (i != 0) { cout <<","; }
            }
        } else {
            for(int i = 0; i < nums.size(); i++) {
                cout << nums[i];
                if (i != nums.size() - 1) { cout <<","; }
            }
        }
        cout << "]\n";
    }

    return 0;
}