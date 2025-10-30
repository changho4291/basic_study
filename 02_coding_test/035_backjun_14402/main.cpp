#include <iostream>
#include <map>
#include <vector>
#include <cstring>

int main(int argc, char const *argv[]) {
    std::cin.tie(NULL);
	std::ios_base::sync_with_stdio(false);

    int q, cnt = 0;
    std::cin >> q;

    std::map<std::string, std::vector<std::string>> records;

    for (int i = 0; i < q; i++) {
        std::string name;
        std::string oper;

        std::cin >> name >> oper;

        if (records.find(name) != records.end()) {
            std::string temp = records[name].back();
            if(oper == "-") {
                if (temp == "-") { cnt++; }
                else { cnt--; }
            } else { cnt++; }
            records[name].push_back(oper);
        } else {
            std::vector<std::string> reco;
            reco.push_back(oper);
            records.insert(make_pair(name, reco));
            cnt++;
        }
    }

    printf("%d", cnt);
    return 0;
}