#include <cstdio>
#include <cstring>

int main(int argc, char const *argv[]) {
    char str[100];
    char cro[8][4] = {"c=", "c-", "dz=", "d-", "lj", "nj", "s=", "z="};

    scanf("%s", str);

    size_t len = strlen(str);

    int cnt = 0;
    char tok[100];
    int tokIndex = 0;
    for (int i = 0; i < len; i++) {
        tok[tokIndex] = str[i];
        tok[tokIndex + 1] = '\0';
        tokIndex++;
        cnt++;
        for (int j = 0; j < 8; j++) {
            if (strstr(tok, cro[j]) != NULL) {
                tokIndex = 0;
                cnt -= strlen(cro[j]);
                cnt++;
                break;
            }
        }
    }
    printf("%d", cnt);
}