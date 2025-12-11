#include <iostream>

using namespace std;

int main(int argc, char const *argv[]) {
    string str;
    cin >> str;

    bool cut_flag = false;
    int end_cnt = 0;
    int stick_cnt = 0;
    int pice = 0;
    for(auto& c : str) {
        if(c == '(') {
            if (cut_flag) {
                cut_flag = false;
                pice += (stick_cnt + end_cnt);

                stick_cnt -= end_cnt;
                end_cnt = 0;
            }
            stick_cnt++;
        } else {
            if (!cut_flag) {
                stick_cnt--;
                cut_flag = true;
            } else {
                end_cnt++;
            }
        }
    }

    if (stick_cnt != 0) {
        pice += (stick_cnt + end_cnt);
    }

    cout << pice << "\n";
    
    return 0;
}