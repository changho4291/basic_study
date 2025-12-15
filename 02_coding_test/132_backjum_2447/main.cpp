#include <iostream>
#include <vector>

using namespace std;

void draw(pair<int, int> idx, int n, vector<vector<char>>& picture);

int main(int argc, char const *argv[]) {
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);

    int n;
    cin >> n;
    vector<vector<char>> picture(n, vector<char>(n, ' '));

    draw({0, 0}, n, picture);

    for (auto& v : picture) {
        for (char& c : v) {
            cout << c;
        }
        cout << "\n";
    }
    
    return 0;
}

void draw(pair<int, int> idx, int n, vector<vector<char>>& picture) {
    if(n/3 == 1) {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (i == 1 && j == 1) { continue; }
                picture[idx.first + i][idx.second + j] = '*';
            }
        }
        return;
    }

    draw(idx, n/3, picture);
    draw({idx.first, idx.second + n/3}, n/3, picture);
    draw({idx.first, idx.second + (n/3 * 2)}, n/3, picture);
    
    draw({idx.first + n/3, idx.second}, n/3, picture);

    draw({idx.first + n/3, idx.second + (n/3 * 2)}, n/3, picture);

    draw({idx.first + (n/3 * 2), idx.second}, n/3, picture);
    draw({idx.first + (n/3 * 2), idx.second + n/3}, n/3, picture);
    draw({idx.first + (n/3 * 2), idx.second + (n/3 * 2)}, n/3, picture);
}


// 0,0 0,3 0,6

// 3,0     3,6

// 6,0 6,3 6,6

// *********
// * ** ** *
// *********
// ***   ***
// * *   * *
// ***   ***
// *********
// * ** ** *
// *********