#include <iostream>
#include <vector>

using namespace std;

typedef struct {
    string name;
    int need;
    int have;
} PokeMon;

int main(int argc, char const *argv[]) {
    cin.tie(0); cout.tie(0);
    ios_base::sync_with_stdio(0);

    int n;
    cin >> n;
    vector<PokeMon> pokemons(n);

    string goat;
    int goatNum = -1;
    int totalEvolution = 0;
    for (auto& p : pokemons) {
        cin >> p.name >> p.need >> p.have;
        int evolution = 0;

        while (p.have >= p.need) {
            int t = (p.have / p.need);
            p.have = (p.have - (t * p.need)) + (t * 2);
            evolution += t;
        }
        totalEvolution += evolution;

        if (evolution > goatNum) {
            goatNum = evolution;
            goat = p.name;
        }
    }

    cout << totalEvolution << "\n";
    cout << goat << "\n";

    return 0;
}