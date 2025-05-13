#include "generic_stack_plus.hpp"
#include <iostream>
#include <stack>

int main(int argc, char const *argv[])
{
    Stack<int> s;

    s.push(10);
    s.push(20);

    while (!s.empty()) {
        std::cout << "Top " << s.top() << std::endl;
        s.pop();
    }

    std::stack<int> st;

    st.push(1);
    st.push(2);
    st.push(3);
    st.push(4);
    st.push(5);

    while (!st.empty()) {
		std::cout << st.top() << std::endl;
		st.pop();
	}
    
    return 0;
}
