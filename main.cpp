
#include "include/gc.hpp"
#include <iostream>
#include <new>

int function() {
    try {
        Smart_ptr<int> array;
        array.set_gclist_collect_size(20);
        for (int i = 0; i < 100; i++)
            array = new int;
        std::cout << "collect";
        array.garbagecollect();
        std::cout << "after";
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