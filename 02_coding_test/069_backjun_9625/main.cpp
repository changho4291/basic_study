#include <iostream>
#include <vector>
#include <map>

using namespace std;

int main(int argc, char const *argv[]) {
    cin.tie(0);
    cout.tie(0);
    ios_base::sync_with_stdio(0);

    int k;
    cin >> k ;
    vector<pair<int,int>> abNum(k);

    abNum[0] = {0, 1};

    for (int i = 1; i < k; i++) {
        abNum[i].first = abNum[i - 1].second;
        abNum[i].second = abNum[i - 1].second;
        abNum[i].second += abNum[i - 1].first;
    }

    cout << abNum[k - 1].first << " " << abNum[k - 1].second << "\n";
    
    return 0;
}