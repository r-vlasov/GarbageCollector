
#include "include/gc.hpp"
#include <iostream>
#include <new>

int function() {
      Smart_ptr<int> A = new int;
      std::cout << (unsigned long long)A.getaddr();
}

int main() {  
    Smart_ptr<int> A[20];
    for (int i = 0; i < 20; i++) {
      A[i] = new int;
    }
    function();
}