#include <iostream>
#include <cstring>

using namespace std;

int main(int argc, char const *argv[]) {
    cin.tie(0);
    ios_base::sync_with_stdio(0);

    int arr[1000];
    int N, K;
    int size, corser = 0;

    cin >> N >> K;
    size = N;

    for (int i = 1; i <= N; i++) {
        arr[i - 1] = i;
    }
    
    cout << "<";
    while(size) {
        corser = (corser + K - 1) % size;
        cout << arr[corser];
        if(size != 1) { cout << ", "; }

        memcpy(arr + corser, arr + corser + 1, sizeof(int) * (size - corser - 1));
        size--;
    }
    cout << ">";

    return 0;
}