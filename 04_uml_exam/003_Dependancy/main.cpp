#include <iostream>

using namespace std;

class Y {
public:
    void foo() {
        cout << "Hello foo\n";
    }
};

class X {
public:
    void function1() {
        Y y;
        cout << "Hello function1\n";
        y.foo();
    }

    void function2() {
        cout << "Hello function2\n";
    }
};

int main(int argc, char const* argv[]) {
    X x;

    x.function1();
    x.function2();

    return 0;
}
