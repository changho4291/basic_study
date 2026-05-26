#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <climits>
#include <utility>

using namespace std;

static inline pair<int,int> rot45_cw(pair<int,int> p) {
    // 좌표계를 2배(정수 격자)로 두었을 때 45도 시계 회전
    // (x, y) -> (x + y, -x + y)
    return {p.first + p.second, -p.first + p.second};
}

static inline int mod360(int k) {
    k %= 360;
    if (k < 0) k += 360;
    return k;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int R, C;
    cin >> R >> C;

    vector<string> a(R);
    for (int i = 0; i < R; i++) cin >> a[i];

    int K;
    cin >> K;
    K = mod360(K);
    int steps = K / 45;  // 0..7

    struct Node { int x, y; char ch; };
    vector<Node> nodes;
    nodes.reserve((size_t)R * C);

    int minx = INT_MAX, maxx = INT_MIN;
    int miny = INT_MAX, maxy = INT_MIN;

    for (int r = 0; r < R; r++) {
        for (int c = 0; c < C; c++) {
            pair<int,int> p = {2*r, 2*c}; // 2배 격자
            for (int s = 0; s < steps; s++) p = rot45_cw(p);

            minx = min(minx, p.first);
            maxx = max(maxx, p.first);
            miny = min(miny, p.second);
            maxy = max(maxy, p.second);

            nodes.push_back({p.first, p.second, a[r][c]});
        }
    }

    int H = maxx - minx + 1;
    int W = maxy - miny + 1;

    vector<string> out(H, string(W, ' '));
    for (auto &nd : nodes) {
        out[nd.x - minx][nd.y - miny] = nd.ch;
    }

    // 줄 수를 최소로: 위/아래 완전 공백 줄 제거 + 각 줄의 trailing space 제거
    auto rstrip = [](string &s) {
        while (!s.empty() && s.back() == ' ') s.pop_back();
    };

    for (auto &line : out) rstrip(line);

    int top = 0;
    while (top < H && out[top].empty()) top++;

    int bottom = H - 1;
    while (bottom >= 0 && out[bottom].empty()) bottom--;

    if (top > bottom) return 0; // 이론상 없음

    for (int i = top; i <= bottom; i++) {
        cout << out[i] << "\n";
    }
    return 0;
}