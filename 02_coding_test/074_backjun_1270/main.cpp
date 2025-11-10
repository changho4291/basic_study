#include <iostream>
#include <sstream>
#include <map>

using namespace std;

int main(int argc, char const *argv[]) {
    cin.tie(0);
    cout.tie(0);
    ios_base::sync_with_stdio(0);

    int n;
    string temp;
    getline(cin, temp);
    n = atoi(temp.c_str());

    for (int i = 0; i < n; i++) {
        int cnt = 0;
        string army;
        stringstream sstream;
        map<string, int> solder;

        getline(cin, army);
        sstream << army;

        while(getline(sstream, army, ' ' )) {
            solder[army]++;
            cnt++;
        }

        bool findFlag = false;
        for (auto& m : solder) {
            if ((float)cnt / m.second <= 2) {
                cout << m.first << "\n";
                findFlag = true;
                break;
            }
        }

        if (!findFlag) {
            cout << "SYJKGW" << "\n";
        }
    }
    
    return 0;
}