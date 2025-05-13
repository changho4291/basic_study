#ifndef __GENERIC_QUEUE_H__
#define __GENERIC_QUEUE_H__

#include <vector>
#include <stdexcept>

template<typename T>
class generic_queue
{
private:
    std::vector<T> data;
public:
    generic_queue() {}
    virtual ~generic_queue() {}

    void enqueue(const T& value) {
        data.push_back(value);
    }

    void dequeue() {
        if (data.empty()) { throw std::out_of_range("Queue empty"); }
        data.erase(data.begin());
    }

    T& front() {
        if (data.empty()) { throw std::out_of_range("Queue empty"); }
        return data.front();
    }

    bool empty() const {
        return data.empty();
    }

    size_t size() const {
        return data.size();
    }
};

#endif // __GENERIC_QUEUE_H__