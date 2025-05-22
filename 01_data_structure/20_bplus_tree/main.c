#include "bpluse_tree.h"
#include <stdio.h>

int main(int argc, char const *argv[]) {
    insert(10, 100);
    insert(20, 200);
    insert(30, 300);
    insert(40, 400);
    insert(25, 250);
    insert(35, 350);
    insert(50, 500);

    delete_key(25);
    delete_key(10);

    print_dot_tree();   //bplus_tree.dot 생성

    return 0;
}
