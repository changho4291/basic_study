#ifndef __GENERIC_STACK_PLUS_H__
#define __GENERIC_STACK_PLUS_H__

#include <vector>
#include <stdexcept>

template<typename T>
class Stack
{
private:
    std::vector<T> data;
public:
    explicit Stack(size_t capacity = 64) { data.reserve(capacity); }

    virtual ~Stack() {}

    void push(const T& value) { data.push_back(value); }

    void pop() {
        if (data.empty()) { throw std::underflow_error("Stack is empty"); }
        data.pop_back();
    }

    T& top() {
        if (data.empty()) { throw std::underflow_error("Stack is empty"); }
        return data.back();
    }

    const T& top() const {
        if (data.empty()) { throw std::underflow_error("Stack is empty"); }
        return data.back;
    }

    bool empty() const { return data.empty(); }

    size_t size() const { return data.size(); }

    void clear() { data.clear(); }
};

#endif // __GENERIC_STACK_PLUS_H__