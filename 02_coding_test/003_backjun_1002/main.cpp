#include <iostream>
#include <vector>

#include <math.h>

int main(int argc, char const *argv[]) {
    
    std::vector<std::vector<int>> test_cases;

    int test_case_num = 0;
    scanf("%d", &test_case_num);

    for (int i = 0; i < test_case_num; i++) {
        int num[6];
        scanf("%d %d %d %d %d %d", num, num + 1, num + 2
            , num + 3, num + 4, num + 5);

        std::vector<int> coordinate;

        coordinate.push_back(num[0]);
        coordinate.push_back(num[1]);
        coordinate.push_back(num[2]);
        coordinate.push_back(num[3]);
        coordinate.push_back(num[4]);
        coordinate.push_back(num[5]);

        test_cases.push_back(coordinate);
    }

    for (int i = 0; i < test_case_num; i++) {
        int a = test_cases[i][0] - test_cases[i][3];
        int b = test_cases[i][1] - test_cases[i][4];

        int r1 = test_cases[i][2];
        int r2 = test_cases[i][5];

        float distance = sqrt(pow(a, 2) + pow(b, 2));   // 두 점의 거리

        printf ("%f\r\n", distance);

        if (r2 - r1 < distance < r2 + r1) {
            printf("두점에서 만난다.\r\n");
        } 
        else if (r1 + r2 == distance) {
            printf("외접 한점에서 만난다.\r\n");
        }
        else if (r2 - r1 == distance) {
            printf("내접 한점에서 만난다.\r\n");
        }
        else if (r1 + r2 < distance) {
            printf("외부에 있다.\r\n");
        }
        else if (r1 - r2 > distance) {
            printf("내부에 있다.");
        }
        else {
            printf("동심원");
        }
    }

    return 0;
}
