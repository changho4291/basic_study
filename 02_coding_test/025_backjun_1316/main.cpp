#include <cstdio>
#include <cstring>

#define NON         0
#define CHECK       1
#define IS_GROUP    2

int main(int argc, char const *argv[]) {
    int n;
    int cnt = 0;

    scanf("%d", &n);

    for (int i = 0; i < n; i++) {
        char str[101];
        char alpa[26] = {NON};
        
    
        scanf("%s", str);
    
        size_t len = strlen(str);
    
        for (int j = 0; j < len; j++) {
            if (alpa[str[j] - 'a'] == NON) {
                alpa[str[j] - 'a'] = CHECK;
    
                if (j > 0 && alpa[str[j-1] - 'a'] == CHECK) {
                    alpa[str[j-1] - 'a'] = IS_GROUP;
                }
            }
            else if (alpa[str[j] - 'a'] == IS_GROUP) {
                cnt++;
                break;
            }
        }
    }

    printf("%d", n - cnt);
}