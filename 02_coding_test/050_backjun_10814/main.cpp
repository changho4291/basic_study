#include <iostream>
#include <vector>

using namespace std;

int main(int argc, char const *argv[]) {
    cin.tie(0);
    ios_base::sync_with_stdio(0);

    int N;
    int age;
    string name;
    vector<vector<string>> members(200);

    cin >> N;

    for (int i = 0; i < N; i++) {
        cin >> age >> name;
        members[age-1].push_back(name);
    }

    age = 0;
    for(vector<string>& m : members) {
        for(string& name : m) {
            cout << age + 1 << " " << name << "\n";
        }
        age++;
    }

    return 0;
}