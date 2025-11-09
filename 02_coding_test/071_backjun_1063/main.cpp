#include <iostream>
#include <vector>

using namespace std;

void movePiece(char& kingX, char& kingY, char& phonX, char& phonY, char row, char column) {
    if (phonX == kingX + row && phonY == kingY + column) {
        if (phonX + row > 'H' || phonX + row < 'A') { return; }
        if (phonY + column > '8' || phonY + column < '1') { return; }
        phonX += row;
        kingX += row;
        phonY += column;
        kingY += column;
    } else {
        if (kingX + row > 'H' || kingX + row < 'A') { return; }
        if (kingY + column > '8' || kingY + column < '1') { return; }
        kingX += row;
        kingY += column;
    }
}

int main(int argc, char const *argv[]) {
    cin.tie(0);
    cout.tie(0);
    ios_base::sync_with_stdio(0);

    char kingX, kingY;
    char phonX, phonY;
    
    int n;
    cin >> kingX >> kingY;
    cin >> phonX >> phonY;

    cin >> n;

    string command;
    for (int i = 0; i < n; i++) {
        cin >> command;

        if (command == "R") {
            movePiece(kingX, kingY, phonX, phonY, 1, 0);
        } else if (command == "L") {
            movePiece(kingX, kingY, phonX, phonY, -1, 0);
        } else if (command == "B") {
            movePiece(kingX, kingY, phonX, phonY, 0, -1);
        } else if (command == "T") {
            movePiece(kingX, kingY, phonX, phonY, 0, 1);
        } else if (command == "RT") {
            movePiece(kingX, kingY, phonX, phonY, 1, 1);
        } else if (command == "LT") {
            movePiece(kingX, kingY, phonX, phonY, -1, 1);
        } else if (command == "RB") {
            movePiece(kingX, kingY, phonX, phonY, 1, -1);
        } else if (command == "LB") {
            movePiece(kingX, kingY, phonX, phonY, -1, -1);
        }
    }

    cout << kingX << kingY << "\n";
    cout << phonX << phonY << "\n";
    
    return 0;
}