#include <iostream>
#include <vector>

using namespace std;

int main(int argc, char const *argv[]) {
    cin.tie(0);
    ios_base::sync_with_stdio(0);

    int M;
    vector<int> S(20, 0);

    cin >> M;

    string command;
    int num;
    for (int i = 0; i < M; i++) {
        cin >> command;

        if (command == "add") {
            cin >> num;
            S[num - 1] = 1;
        } else if (command == "remove") {
            cin >> num;
            S[num - 1] = 0;
        } else if (command == "check") {
            cin >> num;
            cout << S[num - 1] << "\n";
        } else if (command == "toggle") {
            cin >> num;
            S[num - 1] ^= 1;
        } else if (command == "all") {
            for (int i = 0; i < 20; i++) {
                S[i] = 1;
            }
        } else if (command == "empty") {
            for (int i = 0; i < 20; i++) {
                S[i] = 0;
            }
        }
    }

    return 0;
}