
#include "include/gc.hpp"
#include <iostream>
#include <new>

int function() {
    int *p = new int;
    int *c = new int;
    Smart_ptr<int> B[20];
    Smart_ptr<int> A = new int ;
    B[0] = p;
    B[10] = c;
}

int main() { 
    function();

    Smart_ptr<int> C = new int;
}