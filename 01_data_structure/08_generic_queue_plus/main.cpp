#include "generic_queue.hpp"
#include <iostream>

int main(int argc, char const *argv[]) {
    generic_queue<int> q;
    q.enqueue(100);
    q.enqueue(200);
    q.enqueue(300);

    while (!q.empty()) {
        std::cout << "Front: " << q.front() << std::endl;
        q.dequeue();
    }
    
    return 0;
}
