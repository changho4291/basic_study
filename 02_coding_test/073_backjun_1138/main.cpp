#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

bool compaire(pair<int, int> a, pair<int, int> b) {
    if (a.second < b.second) {
        return true;
    } else if ((a.second > b.second)) {
        return false;
    } else {
        return a.first < b.first;
    }
}

int main(int argc, char const *argv[]) {
    cin.tie(0);
    cout.tie(0);
    ios_base::sync_with_stdio(0);

    int n;
    cin >> n;
    vector<pair<int, int>> people(n);
    vector<int> line(n, 0);

    for (int i = 0; i < n; i++) {
        people[i].first = i + 1;
        cin >> people[i].second;
    }

    sort(people.begin(), people.end(), compaire);

    int hight = n;
    for (int i = 0; i < n; i++) {
        for (int j = i; j < n; j++) {
            if (people[j].second == n - hight) {
                if (line[i] == 0) {
                    line[i] = people[j].first;
                } else {
                     
                }
                break;
            }
        }
        hight--;
    }

    for (auto& i : line) {
        cout << i << " ";
    }
    cout << "\n";
    
    return 0;
}