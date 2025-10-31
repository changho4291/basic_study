#include <iostream>

using namespace std;

class User {
private:
    string id;
    string password;

public:
    User() : id("id1"), password("1q2w3e4r") {}
    virtual ~User() {}

    virtual void login() {
        cout << "id: " << id << " password: " << password << "\n";
    }
};

class Customer : public User {
public:
    void login() {
        cout << "customer\n";
    }
};

class Admin : public User {
public:
    void login() {
        cout << "admin\n";
    }
};


int main(int argc, char const* argv[]) {
    User user;
    User* customer = new Customer;
    User* admin = new Admin;

    user.login();
    customer->login();
    admin->login();

    return 0;
}
