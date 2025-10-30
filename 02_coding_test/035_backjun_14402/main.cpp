#include <iostream>
#include <map>
#include <vector>
#include <cstring>

typedef struct {
    std::string oper;
    int in_count = 0;
} Work;

int main(int argc, char const *argv[]) {
    int q, cnt = 0;
    std::cin >> q;

    std::map<std::string, std::vector<Work>> records;

    for (int i = 0; i < q; i++) {
        std::string name;
        std::string oper;

        std::cin >> name;
        std::cin >> oper;

        Work newWork;
        newWork.oper = oper;
        if (records.find(name) != records.end()) {
            Work temp = records[name].back();
            if(oper == "-") {
                if (temp.oper == "-") { cnt++; }
                newWork.in_count = 0;
            } else {
                if (temp.oper == "+") { 
                    if (temp.in_count == 1) { cnt += 2; }
                    else { cnt++; }
                    newWork.in_count = temp.in_count + 1;
                }
            }
            records[name].push_back(newWork);
        } else {
            if(oper == "-") { cnt++; }
            else { newWork.in_count = 1; }
            std::vector<Work> reco;
            reco.push_back(newWork);
            records.insert(make_pair(name, reco));
        }
    }

    printf("%d", cnt);

    return 0;
}