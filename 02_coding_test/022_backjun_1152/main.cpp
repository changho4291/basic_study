#include <stdio.h>
#include <string.h>

int main(int argc, char const *argv[]) {
    char str[1000000];

    scanf("%[^\n]s", str);

    int cnt = 0;
    char* tok = strtok(str, " ");
    while (tok != NULL) {
        tok = strtok(NULL, " ");
        cnt++;
    }
    printf("%d", cnt);
}