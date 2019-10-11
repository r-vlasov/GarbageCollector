#ifndef __GC
#define __GC

#include <list>
#include <stddef.h>
#include "iterator.hpp"
#include "info.hpp"


template <class T, int size = 0>
class Smart_ptr {
    static list<Info<T> > gclist;
    T* addr;
    unsigned length; // if it is a array -> size of array, else size = 1;
    union {
        bool is_array;
        bool check;
    };
public:
    // define an iterator for gc
    Smart_ptr(T* t = NULL);

    // rule of three :)
    Smart_ptr(const Smart_ptr<T, size>& obj); // copy constructor
    Smart_ptr& operator= (Smart_ptr<T, size> &rv);
    ~Smart_ptr(); // destructor;

    list<Info<T> >::iterator findInfoInList(T* ptr);

    T& operator* ();
    T* operator->();
    T& operator[](int i);
    
    bool garbagecollect();
    //operator T*();
};
#endif