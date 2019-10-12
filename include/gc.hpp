#ifndef __GC
#define __GC

#include <list>
#include <stddef.h>
//#include "iterator.hpp"
#include "info.hpp"
#include <iostream>

template <class T, int size = 0>
class Smart_ptr {
    static std::list<Info<T> > gclist;
    T* addr;
    unsigned length; // if it is a array -> size of array, else size = 0;
    union {
        bool is_array;
        bool check;
    };
public:
    // iterator on ptr in list
    typename std::list<Info<T> >::iterator findInfoInList(T* ptr);

    Smart_ptr(T* t = NULL);

    // rule of three :)
    Smart_ptr(const Smart_ptr<T, size>& obj); // copy constructor
    Smart_ptr& operator= (T* value);
    Smart_ptr& operator= (Smart_ptr<T, size> &rv);
    ~Smart_ptr(); // destructor;

    T& operator* ();
    T* operator->();
    T& operator[](int i);
    
    bool garbagecollect();
        
    T* getaddr() const;
    int getlength() const;
};

// out class is templated, so we should define this
template <class T, int size> 
std::list<Info<T> > Smart_ptr<T, size>::gclist;

template <class T, int size>
typename std::list<Info<T> >::iterator Smart_ptr<T, size>::findInfoInList(T* t){
    typename std::list<Info<T> >::iterator p;
    for (p = gclist.begin(); p != gclist.end(); p++) {
        if (p->getallocmem() == t)
            return p;
    }
    return p;
}

template <class T, int size> 
Smart_ptr<T, size>::Smart_ptr(T* t) {
    typename std::list<Info<T> >::iterator p;
    p = findInfoInList(t);
    if (p != gclist.end())
        p->increfcount();
    else {
        Info<T> obj(t, size);
        gclist.push_front(obj);
    }
    addr = t;
    length = size;
    if (size > 0) 
        is_array = true;
    else 
        is_array = false;
}

template <class T, int size>
Smart_ptr<T, size>::Smart_ptr(const Smart_ptr<T, size>& obj) {
    typename std::list<Info<T> >::iterator p;
    p = findInfoInList(obj.getaddr());
    p->increfcount();
    addr = obj.getaddr();
    length = obj.getlength();
    if (length > 0)
        is_array = true;
    else
        is_array = false;
}

template <class T, int size>
Smart_ptr<T, size>& Smart_ptr<T, size>::operator= (T* value) {
    typename std::list<Info<T> >::iterator p;
    p = findInfoInList(this->addr);
    if (p != gclist.end()) {
        p->decrefcount();
        p = findInfoInList(value);
        p->increfcount();
        this->addr = value;
    }
    else {
        //throw
    }
}

template <class T, int size>
Smart_ptr<T, size>::~Smart_ptr() {
    typename std::list<Info<T> >::iterator p;
    p = findInfoInList(addr);
    if (p->getrefcount())
        p->decrefcount();
    garbagecollect();
}

template <class T, int size>
T& Smart_ptr<T, size>::operator* () {
    return *this;
}

template <class T, int size>
T* Smart_ptr<T, size>::operator->() {
    return this;
}

template <class T, int size>
T& Smart_ptr<T, size>::operator[](int i) {
    return addr[i];
}

template <class T, int size>
bool Smart_ptr<T, size>::garbagecollect() {
    typename std::list<Info<T> >::iterator p;
    bool freed = false;
    do {
        for (p = gclist.begin(); p != gclist.end(); p++) {
            if(p->getrefcount() > 0)
                continue;
            freed = true;
            
            std::cout << std::endl <<this->gclist.size() << std::endl;

            gclist.remove(*p); 
            if(p->getallocmem()) {
                if (p->getisarray()) {
                    std::cout << (unsigned long long) p->getallocmem() << std::endl;
                    std::cout << this->gclist.size() << std::endl;
                    delete[] p->getallocmem();
                }
                else {
                    delete p->getallocmem();
                }
            }
            break;   
        }
    } while (p!= gclist.end());
    return freed;
}

template <class T, int size>
T* Smart_ptr<T, size>::getaddr() const {
    return addr;
}

template <class T, int size>
int Smart_ptr<T, size>::getlength() const {
    return length;
}

#endif
