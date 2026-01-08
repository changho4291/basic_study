#include <iostream>
#include <vector>

using namespace std;

int main(int argc, char const *argv[]) {
    cin.tie(0);
    cout.tie(0);
    ios_base::sync_with_stdio(0);

    int a, b;
    cin >> a >> b;
    vector<vector<int>> feed(b, vector<int>(a));
    for (int i = 0; i < b; i++) {
        for (int j = 0; j < a; j++) {
            cin >> feed[i][j];
        }
    }

    int c, fail = 0;
    for (int i = 0; i < a; i++) {
        for (int j = b - 1; j >= 0; j--) {
            cin >> c;
            if (feed[j][i] != c) {
                fail = 1;
            }
        }
    }

    if (fail) {
        cout << "|>___/|     /}\n"
                "| O O |    / }\n"
                "( =0= )\"\"\"\"  \\\n"
                "| ^  ____    |\n"
                "|_|_/    ||__|\n";
    } else {
        cout << "|>___/|        /}\n"
                "| O < |       / }\n"
                "(==0==)------/ }\n"
                "| ^  _____    |\n"
                "|_|_/     ||__|\n";
    }

    return 0;
}
