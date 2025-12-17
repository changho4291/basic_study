// d, n 이 어느 좌표에 있는지 확인되면
// x, y 의 값도 역산해서 몇번째 사분면에 있는지 확인 가능?

#include <iostream>
#include <cstdint>
#include <cmath>

using namespace std;

int64_t id, ix, iy;
string n, result = "";

pair<int64_t, int64_t> findXY(const int64_t d, const string& n, int64_t x, int64_t y, int64_t len);
void findNum(const int64_t d, pair<int64_t, int64_t>& xy, int64_t x, int64_t y, int64_t len);

int main(int argc, char const *argv[]) {
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);

    cin >> id >> n >> ix >> iy;

    int64_t len = pow(2, id);
    auto xy = findXY(id, n, 0, 0, len);

    xy.first += ix;
    xy.second -= iy;

    if(xy.first < 0 || xy.second < 0 || xy.first >= len || xy.second >= len) {
        cout << "-1\n";
        return 0;
    }
    
    findNum(0, xy, 0, 0, len);
    cout << result;
    return 0;
}

pair<int64_t, int64_t> findXY(const int64_t d, const string& n, int64_t x, int64_t y, int64_t len) {
    if (d == 0) { return {x, y}; }

    char c = n[id - d];
    if (c == '1') { return findXY(d - 1, n, x + (len/2),y ,len/2); }
    else if (c == '2') { return findXY(d - 1, n, x ,y ,len/2); }
    else if (c == '3') { return findXY(d - 1, n, x ,y + (len/2) ,len/2); }
    return findXY(d - 1, n, x + (len/2) ,y + (len/2) ,len/2);
}

void findNum(const int64_t d, pair<int64_t, int64_t>& xy, int64_t x, int64_t y, int64_t len) {
    if(d == id) { result += "\n"; return; }

    if (xy.first >= x + len/2 && xy.second < y + len/2) {
        result += "1"; 
        findNum(d + 1, xy, x + len/2, y, len/2);
        return;
    } else if (xy.first < x + len/2 && xy.second < y + len/2) { 
        result += "2"; 
        findNum(d + 1, xy, x, y, len/2);
    } else if (xy.first < x + len/2 && xy.second >= y + len/2) {
        result += "3"; 
        findNum(d + 1, xy, x, y + len/2, len/2);
    } else {
        result += "4"; 
        findNum(d + 1, xy, x + len/2, y + len/2, len/2);
    }
}