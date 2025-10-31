#include <iostream>

using namespace std;

class Address {
public:
   Address() {}
   ~Address() {}
};

class User {
private:
    Address* addr;
public:
    User(Address* addr_) : addr(addr_) {}
    ~User() {
        delete addr;    // User 파괴시 Address도 파괴
    }
};

int main(int argc, char const* argv[]) {
    Address* addr = new Address;
    User user(addr);
    return 0;
}
