#include <iostream>
#include <vector>
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
        vector<char> nums(n);
        
        // 배열 파싱
        if (n > 0) { nums[0] = arr[1]; }
        
        for(int i = 1, j = 2; i < n; i++, j+=2) {
            nums[i] = arr[1 + j];
        }
        
        // 커맨드 압축
        vector<char> reCommand;
        int cntR = 0;
        int cntD = 0;
        char prevF = 0;
        for (char& c : command) {
            if (c == 'R') {
                cntR++;
            } else {
                cntD++;
                if (prevF == 'R') {
                    if ( (cntR % 2) != 0) { reCommand.push_back('R'); }
                    cntR = 0;
                }
                reCommand.push_back('D');
            }
            prevF = c;
        }

        if (cntR != 0) {
            if ( (cntR % 2) != 0) { reCommand.push_back('R'); }
        }

        if (cntD > n) {
            cout << "error\n";
            continue;
        }

        for(char& c : reCommand) {
            if (c == 'R') {
                reverse(nums.begin(), nums.end());
            } else {
                nums.erase(nums.begin());
            }
        }

        cout << "[";
        for(int i = 0; i < nums.size(); i++) {
            cout << nums[i];
            if (i != nums.size() - 1) { cout <<","; }
        }
        cout << "]\n";
    }

    return 0;
}