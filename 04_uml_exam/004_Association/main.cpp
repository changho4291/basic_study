#include <iostream>

using namespace std;

class Address {
public:
   Address() {}
   ~Address() {}
};

class User {
private:
    Address& addr;
public:
    User(Address& addr_) : addr(addr_) {}
    ~User() {}
};

int main(int argc, char const* argv[]) {
    Address addr;
    User user(addr);
    return 0;
}
