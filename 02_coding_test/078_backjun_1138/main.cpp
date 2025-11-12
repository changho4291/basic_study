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
        int taller = people[i];  // 왼쪽에 있어야 하는 키 큰 사람 수
        int cnt = 0;
        for (int pos = 0; pos < n; ++pos) {
            if (line[pos] == 0) {          // 빈 자리만 센다
                if (cnt == taller) {       // 필요한 만큼 빈칸을 건너뛰었으면
                    line[pos] = i + 1;     // 여기 앉는다 (키 = i+1)
                    break;
                }
                cnt++;
            }
        }
    }

    for (auto& i : line) {
        cout << i << " ";
    }
    cout << "\n";
    
    return 0;
}