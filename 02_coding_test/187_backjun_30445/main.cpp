#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

int main(int argc, char const *argv[]) {
    cin.tie(0); cout.tie(0);
    ios_base::sync_with_stdio(0);

    string word;
    getline(cin, word);

    double ph = 0, pg = 0;

    char happy[] = "HAPPY";
    char sad[] = "SAD";

    for(char& c : word) {
        for(int i = 0; i < 5; i++) {
            if (happy[i] == c) {
                ph++;
                break;
            }
        }

        for(int i = 0; i < 3; i++) {
            if (sad[i] == c) {
                pg++;
                break;
            }
        }
    }

    if (ph == 0 && pg == 0) {
        cout << "50.00\n";
    } else {
        cout << fixed;
	    cout.precision(2);
        cout << round((ph / (ph + pg)) * 10000) / 100 << "\n";
    }

    return 0;
}