#include <iostream>

using namespace std;

class Service {
public:
    virtual void getObject();
};

class ServiceImpl : public Service {
public:
    void getObject() {
        cout << "ServiceImpl\n";
    }
};

int main(int argc, char const* argv[]) {
    Service* service = new ServiceImpl;

    service->getObject();

    return 0;
}
