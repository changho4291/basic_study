#include <iostream>
#include <vector>
#include <cstdint>

using namespace std;

int main(int argc, char const *argv[]) {
    cin.tie(0);
    cout.tie(0);
    ios_base::sync_with_stdio(0);

    vector<uint64_t> wave;

    wave.push_back(1);
    wave.push_back(1);
    wave.push_back(1);
    wave.push_back(2);
    wave.push_back(2);

    int n, m;
    cin >> n;

    for(int i = 0; i < n; i++) {
        cin >> m;

        if(m - 1 < wave.size()) { 
            cout << wave[m - 1] << "\n";
            continue;
        }
        for (int j = wave.size(); j < m; j++) {
            wave.push_back(wave[j - 1] + wave[j - 5]);
        }
        cout << wave[m - 1] << "\n";
    }

    return 0;
} 