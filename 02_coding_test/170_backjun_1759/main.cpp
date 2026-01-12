#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int l, c;
char arr[16] = {0, };
char alpas[16] = {0 ,};
bool isUsed[16] = {false, };

char vowel[] = {'a', 'e', 'i', 'o', 'u'};
int vowels = 0;     // 모음 수
int consonant = 0;  // 자음 수

void backTrac(int idx, int pre);

int main(int argc, char const *argv[]) {
    cin.tie(0);
    cout.tie(0);
    ios_base::sync_with_stdio(0);

    cin >> l >> c;

    for (int i = 0; i < c; i++) { cin >> alpas[i]; }
    sort(alpas, alpas + c);

    backTrac(0, 0);
}

void backTrac(int idx, int pre) {
    if (idx == l && vowels > 0 &&  consonant > 1) {
        for (int i = 0; i < l; i ++) {
            cout << arr[i];
        }
        cout << "\n";
    }

    for (int i = pre; i < c; i++) {
        if (isUsed[i]) { continue; }
        isUsed[i] = true;
        arr[idx] = alpas[i];

        bool isVowel = false;
        for (int j = 0; j < 5; j++) {
            if (arr[idx] == vowel[j]) {
                vowels++;
                isVowel = true;
            }
        }

        if (!isVowel) {
            consonant++;
        }

        backTrac(idx + 1, i + 1);
        isUsed[i] = false;
        
        if (isVowel) {
            vowels--;
        } else {
            consonant--;
        }
    }
}
