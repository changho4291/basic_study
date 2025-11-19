#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdint>

using namespace std;

bool compare(const pair<uint32_t,uint32_t>& a, const pair<uint32_t,uint32_t>& b) {
    if (a.second > b.second) {
        return false;
    } else if (a.second == b.second) {
        if (a.first > b.first) {
            return false;
        }
        return true;
    } 
    return true;
}

int main(int argc, char const *argv[]) {
    cin.tie(0);
    cout.tie(0);
    ios_base::sync_with_stdio(0);

    int n;
    cin >> n;
    vector<pair<uint32_t, uint32_t>> meetings(n);
    for (auto& m : meetings) {
        cin >> m.first >> m.second;
    }

    if (n == 0) {            // 빈 입력 방어
        cout << 0 << '\n';
        return 0;
    }

    sort(meetings.begin(), meetings.end(), compare);

    pair<uint32_t, uint32_t> temp = meetings[0];
    uint32_t cnt = 1;
    for(uint32_t i = 1; i < n; i++) {
        if (temp.second <= meetings[i].first) {
            cnt++;
            temp = meetings[i];
        }
    }

    cout << cnt << "\n";
    return 0;
}