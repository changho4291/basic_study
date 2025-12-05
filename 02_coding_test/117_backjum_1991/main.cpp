#include <iostream>
#include <vector>

using namespace std;

void preOrder(vector<pair<char, char>>& binaryTree, char root) {
    if (root == '.') { return; }
    cout << root;
    preOrder(binaryTree, binaryTree[root - 'A'].first);
    preOrder(binaryTree, binaryTree[root - 'A'].second);
}

void inOrder(vector<pair<char, char>>& binaryTree, char root) {
    if (root == '.') { return; }
    inOrder(binaryTree, binaryTree[root - 'A'].first);
    cout << root;
    inOrder(binaryTree, binaryTree[root - 'A'].second);
}

void lastOrder (vector<pair<char, char>>& binaryTree, char root) {
    if (root == '.') { return; }
    lastOrder(binaryTree, binaryTree[root - 'A'].first);
    lastOrder(binaryTree, binaryTree[root - 'A'].second);
    cout << root;
}

int main(int argc, char const *argv[]) {
    cin.tie(0);
    cout.tie(0);
    ios_base::sync_with_stdio(0);

    int n;
    cin >> n;
    vector<pair<char, char>> binaryTree(n);
    
    for(int i = 0; i < n; i++) {
        char node;
        cin >> node;
        cin >> binaryTree[node - 'A'].first >> binaryTree[node - 'A'].second;
    }

    // 전위 순회
    preOrder(binaryTree, 'A'); cout << "\n";
    // 중위 순회
    inOrder(binaryTree, 'A'); cout << "\n";
    // 후위 순회
    lastOrder(binaryTree, 'A'); cout << "\n";

    return 0;
}