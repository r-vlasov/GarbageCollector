
#include "include/gc.hpp"
#include <iostream>
#include <new>

int function() {
    try {
        Smart_ptr<Smart_ptr<int>* > a = new Smart_ptr<int>*;
        Smart_ptr<Smart_ptr<int>* > b;

        a = b;
        b = a;

    }
    catch (Smart_iter_exception& exc) {
        std::cout << "1";
    }
    catch (Smart_gc_exception& exc) {
        std::cout << "2";
    }
}

int main() { 
    function();
    std::cout << "mark\n";
}

