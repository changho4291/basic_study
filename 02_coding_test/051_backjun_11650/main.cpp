#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

typedef struct {
    int x = 0;
    int y = 0;
} Coordinate;

int sortFuncion(const void* co1, const void* co2) {
    Coordinate coordi1 = *static_cast<const Coordinate*>(co1);
    Coordinate coordi2 = *static_cast<const Coordinate*>(co2);

    if (coordi1.x != coordi2.x) {
        if (coordi1.x > coordi2.x) { return 1; }
        return -1;
    } else {
        if (coordi1.y > coordi2.y) { return 1; }
        else if (coordi1.y < coordi2.y) { return -1; }
        return 0;
    }
}

int main(int argc, char const *argv[]) {
    cin.tie(0);
    ios_base::sync_with_stdio(0);

    int N, x, y;
    cin >> N;
    vector<Coordinate> coordinates(N);

    for (int i = 0; i < N; i++) {
        cin >> coordinates[i].x >> coordinates[i].y;
    }

    qsort(coordinates.data(), coordinates.size()
        , sizeof(decltype(coordinates)::value_type), sortFuncion);

    for(Coordinate& co : coordinates) {
        cout << co.x << " " << co.y << "\n";
    }

    return 0;
}