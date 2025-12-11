#include <iostream>
#include <stack>

using namespace std;

int main(int argc, char const *argv[]) {
    string str;
    cin >> str;

    // stack<char> st;
    bool cut_flag = false;
    int end_cnt = 0;
    int stick_cnt = 0;
    int pice = 0;
    for(auto& c : str) {
        if(c == '(') {
            if (cut_flag) {
                cut_flag = false;
                cout << stick_cnt << " " << end_cnt << "\n";
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
            // st.pop();
            // pice += (stick_cnt*2);
        }
    }

    if (stick_cnt != 0) {
        cout << stick_cnt << " " << end_cnt << "\n";
        pice += (stick_cnt + end_cnt);
    }

    cout << pice << "\n";
    
    return 0;
}