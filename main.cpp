
#include "include/gc.hpp"
#include <iostream>
#include <new>

int function() {

    int *p = new int;
   // int *d = new int;
    try {
        Smart_ptr<int> array[20];
        array->set_gclist_collect_size(20);
        for (int i = 0; i < 20; i++) {
            array[i] = new int;
        }
    }
    catch (Smart_iter_exception& exc) {
        std::cout << "hello";
    }
    catch (Smart_gc_exception& exc) {
        std::cout << "hello";
    }
}

int main() { 
    function();
}