#include <iostream>
#include <cmath>
#include <cstdint>

using namespace std;

int main(int argc, char const *argv[]) {
    cin.tie(0);
    ios_base::sync_with_stdio(0);

    uint64_t N;
    uint64_t tSize[6];
    uint64_t t, p;

    uint64_t tPackNum = 0;
    uint64_t pPackNum = 0;
    uint64_t pNum = 0;

    cin >> N;
    for (int i = 0; i < 6; i++) {
        cin >> tSize[i];
    }
    
    cin >> t >> p;
    for (int i = 0; i < 6; i++) {
        tPackNum += ceil((double)tSize[i]/t);
    }

    pPackNum = N / p;
    pNum = N % p;

    cout << tPackNum << "\n";
    cout << pPackNum << " " << pNum << "\n";

    return 0;
}