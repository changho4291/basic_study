#include <iostream>

int main(int argc, char const *argv[]) {
    while (1) {
        int num[3];
        
        std::cin >> num[0];
        int max = 0;
        for(int i = 1; i < 3; i++) {
            std::cin >> num[i];
            if(num[max] < num[i]) { max = i; }
        }

        if(!num[0] || !num[1] || !num[2]) { break; }

        num[max] *= num[max];
        int sum = 0;
        for(int i = 0; i < 3; i++) {
            if (i != max) { 
                sum += num[i] * num[i];
            } 
        }

        if(num[max] == sum) { std::cout << "right\n"; }
        else { std::cout << "wrong\n"; }
    }
    
    return 0;
}