#include <iostream>
#include <math.h>

int main(int argc, char const *argv[]) {

    int test_case_num = 0;
    scanf("%d", &test_case_num);

    for (int i = 0; i < test_case_num; i++) {
        int num[6];
        int result = 0;

        scanf("%d %d %d %d %d %d", num, num + 1, num + 2
            , num + 3, num + 4, num + 5);

        int a = num[0] - num[3];
        int b = num[1] - num[4];

        int r1 = num[2];
        int r2 = num[5];

        double distance = sqrt(pow(a, 2) + pow(b, 2));   // 두 점의 거리
        int subtract = abs(r2 - r1);
        int sum = r2 + r1;

        if (distance == 0 && r1 == r2) {   // 두 원이 일치
            printf("-1\n");
        }
        else if (subtract < distance && distance < sum) { // 두 원의 교점이 2개
            printf("2\n");
        } 
        else if (sum == distance || subtract == distance) {  // 두 원의 교점이 1개
            printf("1\n");
        }
        else {
            printf("0\n");
        }
    }

    return 0;
}