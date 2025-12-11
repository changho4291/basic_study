#include <iostream>
#include <vector>

using namespace std;

typedef struct {
    int n, m;
    vector<bool> isUsed;
    vector<int> arr;
} Members;

void backTrack(vector<vector<int>>& ability, Members& mem, int idx, int& min);

int main(int argc, char const *argv[]) {
    int n;
    cin >> n;

    Members mem = {
        .n = n,
        .m = n/2,
        .isUsed = vector<bool>(n, false),
        .arr = vector<int>(n + 1)
    };
    
    vector<vector<int>> ability(n, vector<int>(n));
    for(auto& i : ability) {
        for (auto& j : i) {
            cin >> j;
        }
    }

    mem.arr[0] = 1;
    mem.isUsed[1] = true;

    int min = 100000000;
    backTrack(ability, mem, 1, min);

    cout << min << "\n";
    
    return 0;
}

void backTrack(vector<vector<int>>& ability, Members& mem, int idx, int& min) {
    if (idx == mem.m) {
        int bmember[mem.m];
        for (int i = 1, j = 0; i <= mem.n; i++) {
            if (!mem.isUsed[i]) {
                bmember[j] = i;
                j++;
            }
        }

        int temp = 0;
        int ateam = 0, bteam = 0;
        for (int i = 0; i < (mem.n / 2); i++) {
            for (int j = i + 1; j < (mem.n / 2); j++) {
                // cout << mem.arr[i] << " " << mem.arr[j] << " : " << bmember[i] << " " << bmember[j] << "\n";
                // cout << ability[mem.arr[i] - 1][mem.arr[j] - 1] << " " << ability[mem.arr[j] - 1][mem.arr[i] - 1] 
                //     << " : " << ability[bmember[i] - 1][bmember[j] - 1] << " " << ability[bmember[j] - 1][bmember[i] - 1] << "\n";
                ateam += (ability[mem.arr[i] - 1][mem.arr[j] - 1] + ability[mem.arr[j] - 1][mem.arr[i] - 1]);
                bteam += (ability[bmember[i] - 1][bmember[j] - 1] + ability[bmember[j] - 1][bmember[i] - 1]);

                // cout << temp << "\n";
            }
        }
        
        if (ateam > bteam) {
            temp = ateam - bteam;
        } else {
            temp = bteam - ateam;
        }
        
        if(min > temp) { min = temp; }
        
        // for (int i = 0; i < (mem.n / 2); i++) {
        //     for (int j = i + 1; j < (mem.n / 2); j++) {
        //         cout << bteam[i] << " " << bteam[j] << "\n";
        //     }
        // }

        // cout << "\n";
        return;
    }

    for (int i = 2; i <= mem.n; i++) {
        if (mem.isUsed[i]) { continue; }
        mem.isUsed[i] = true;
        mem.arr[idx] = i;
        backTrack(ability, mem, idx + 1, min);
        mem.isUsed[i] = false;
    }
}