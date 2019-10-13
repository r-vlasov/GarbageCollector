
#include "include/gc.hpp"
#include <iostream>
#include <new>

int function() {
    int *p = new int;
    int *c = new int;
    Smart_ptr<int, 30> B;

    try {
        int i = 0;
        for (Smart_Iterator<int> iter = B.begin(); iter < B.end(); ++iter) {
            std::cout << i << " ";
            i+=1;        
        }
       // if (B > B);
    }
    catch (Smart_iter_exception& exc) {
        std::cout << "hello";
    }
}

int main() { 
    function();
}