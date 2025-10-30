#include <iostream>
#include <map>
#include <vector>
#include <cstring>

int main(int argc, char const *argv[]) {
    int q, cnt = 0;
    std::cin >> q;

    std::map<std::string, std::vector<char>> records;

    for (int i = 0; i < q; i++) {
        std::string name;
        char oper;

        std::cin >> name >> oper;

        if (records.find(name) != records.end()) {
            char temp = records[name].back();
            if(oper == '-') {
                if (temp == '-') { cnt++; }
                else { cnt--; }
            } else { cnt++; }
            records[name].push_back(oper);
        } else {
            records[name].push_back(oper);
            cnt++;
        }
    }

    printf("%d\n", cnt);
    return 0;
}