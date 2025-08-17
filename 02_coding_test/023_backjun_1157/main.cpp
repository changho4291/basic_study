#include <cstdio>
#include <cctype>
#include <cstring>

int main(int argc, char const *argv[]) {
    char str[1000000];
    int cnt[26] = {0};

    scanf("%s", str);
    
    size_t len = strlen(str);
    for (size_t i = 0; i < len; i ++) {
        str[i] = toupper(str[i]);
        
        cnt[str[i] - 65]++;
    }

    int max = 0;
    char maxAlpa;

    for (int i = 0; i < 26; i++) {
        if (max == cnt[i]) {
            maxAlpa = '?';
        } else if (max < cnt[i]) {
            max = cnt[i];
            maxAlpa = i + 65;
        }
    }

    printf("%c", maxAlpa);
}