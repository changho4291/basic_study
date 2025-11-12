#include <iostream>
#include <vector>

using namespace std;

int main(int argc, char const *argv[]) {
    cin.tie(0);
    cout.tie(0);
    ios_base::sync_with_stdio(0);

    int n;
    cin >> n;

    vector<int> people(n);
    vector<int> line(n, 0);

    for (int i = 0; i < n; i++) {
        cin >> people[i];
    }

    for (int i = 0; i < n; i++) {
        if (line[people[i]] == 0) {
            line[people[i]] = i + 1;
        } else {
            int j = 1;
            while (people[i] + j < n) {
                if (line[people[i] + j] == 0) {
                    line[people[i] + j] = i + 1;
                    break;
                }
                j++;
            }
        }
    }

    for (auto& i : line) {
        cout << i << " ";
    }

    cout << "\n";
    
    return 0;
}