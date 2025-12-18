#include <iostream>
#include <algorithm>

using namespace std;

int main () {
    cin.tie(0);
    cout.tie(0);
    ios_base::sync_with_stdio(0);

    string result = "";
    char words[5][16] = {{'\0', }, };

    for (int i = 0; i < 5; i++) { cin >> words[i]; }

    for (int i = 0; i < 15; i++) {
        for (int j = 0; j < 5; j++) {
            if (words[j][i] != '\0') {
                result += words[j][i];
            }
        }
    }

    cout << result << "\n";

    return 0;
}