#include <iostream>
#include <cstring>

using namespace std;

int main(int argc, char const *argv[]) {
    cin.tie(0);
    ios_base::sync_with_stdio(0);

    int N, cnt = 1, num = 666;
    char s[11];

    cin >> N;

    while(N != cnt) {
        num++;
        sprintf(s, "%d", num);
        if (strstr(s, "666") != NULL ) { cnt++; }
    }

    cout << num << "\n";

    return 0;
}