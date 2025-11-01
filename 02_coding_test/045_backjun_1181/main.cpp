#include <iostream>
#include <algorithm>
#include <cstring>

using namespace std;

typedef struct {
    int length = 0;
    char str[51];
} myString;

int comp(const void* s1, const void* s2) {
    myString* str1 = (myString*)s1;
    myString* str2 = (myString*)s2;

    if(str1->length == str2->length) {

        int result = strcmp(str1->str, str2->str);

        // 중복 제거 (중복값을 대문자 A로 변경)
        if (!result) {
            strcpy(str1->str, "A\0");
        }

        return result;
    }

    return str1->length - str2->length;
}

int main(int argc, char const *argv[]) {
    cin.tie(0);
    ios_base::sync_with_stdio(0);

    int no;
    cin >> no;

    // 자료 크기 할당
    myString* strs = new myString[no];
    
    // 값 삽입
    for (int i = 0; i < no; i++) {
        cin >> strs[i].str;
        strs[i].length = strlen(strs[i].str);
    }

    // 정렬
    qsort(strs, no, sizeof(myString), comp);

    for (int i = 0; i < no; i++) {
        if (!strcmp("A", strs[i].str)) { continue; }    // 중복값이라면 출력하지 않음
        cout << strs[i].str << "\n";
    }

    delete[] strs;

    return 0;
}