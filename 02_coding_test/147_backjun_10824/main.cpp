#include <iostream>

using namespace std;

int main(int argc, char const *argv[]) {
    cin.tie(0);
    cout.tie(0);
    ios_base::sync_with_stdio(0);

    string a, b, c, d;

    cin >> a >> b >> c >> d;
    
    a += b;
    c += d;

    cout << atoll(a.c_str()) + atoll(c.c_str()) << "\n";
    
    return 0;
}
