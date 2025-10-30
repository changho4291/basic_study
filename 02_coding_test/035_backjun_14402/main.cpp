#include <iostream>
#include <map>
#include <vector>
#include <cstring>

typedef struct {
    int count = 0;
    char oper;
} Worker;

int main(int argc, char const *argv[]) {
    int q, cnt = 0;
    std::cin >> q;

    std::map<std::string, Worker> records;

    for (int i = 0; i < q; i++) {
        std::string name;
        char oper;

        std::cin >> name >> oper;

        if(records[name].count == 0){
            records[name].count = 1;
            cnt++;
        } else {
            if(oper == '-') {
                if (records[name].oper == '+') { cnt--; }
                else { cnt++; }
            } else { cnt++; }
        }
        records[name].oper = oper;
    }

    std::cout << cnt << "\n";
    return 0;
}