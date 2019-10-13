#ifndef __MYEXCEPTION
#define __MYEXCEPTION

#include <exception>
#include <iostream>

class Smart_exception {
protected:
    const char* _msg;
public: 
    Smart_exception() = default;
    Smart_exception(const char* str) : _msg(str) {
        std::cout << "Exception: ";
    }
};


class Smart_iter_exception: protected Smart_exception{
    Smart_iter_exception(const char* str) {
        this->_msg = str;
        std::cout << "Iterator exception: ";
    }
};

class Smart_gc_exception : protected Smart_exception{
    Smart_gc_exception(const char* str)  {
        this->_msg = str;
        std::cout << "GC exception: ";
    }
};

#endif
