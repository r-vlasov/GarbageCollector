
#include "include/gc.hpp"
#include <iostream>
#include <new>

int function() {
    try {
        Smart_ptr<int, 200> Array = new int[200];
        Array.iterator = Array.begin();
        for (Array.iterator;  Array.iterator<Array.end(); ++Array.iterator) {
            *Array.iterator = 2;
        }
        for (int i = 0; i < 200; i++) {
            std::cout << Array[i];
        }
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
}