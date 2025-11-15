#include <iostream>
#include <vector>

using namespace std;

int main(int argc, char const *argv[]) {
    cin.tie(0);
    cout.tie(0);
    ios_base::sync_with_stdio(0);

    int n, m;
    cin >> n >> m;
    
    vector<int> sum(n, 0);

    int num;
    cin >> num;
    sum[1] = num;
    for(int i = 2; i < n + 1; i++) {
        cin >> num;
        sum[i] = sum[i - 1] + num;
    }

    for (int i = 0; i < m; i++) {
        int start, end, result;
        cin >> start >> end;

        result = sum[end] - sum[start - 1];

        cout << result << "\n";
    }

    return 0;
}