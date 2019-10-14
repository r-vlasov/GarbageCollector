#ifndef __GC
#define __GC

#include <list>
#include <stddef.h>
#include "iterator.hpp"
#include "exception.hpp"
#include "info.hpp"
#include <iostream>


#define MAGIC_SIZE 10
template <class T, int size = 0>
class Smart_ptr {
    static std::list<Info<T> > gclist;
    T* addr;
    unsigned length; // if it is a array -> size of array, else size = 0;
    union {
        bool is_array;
        bool check;
    };

    // the limit size after which the garbagecollect will be called when trying to increase the size of the gclist 
    unsigned gclist_collect_size = MAGIC_SIZE;

    // function that add Info object at gclist
    void add_to_gclist(T* data);
    
    // iterator on ptr in list
    typename std::list<Info<T> >::iterator findInfoInList(T* ptr);

public:
    Smart_ptr(T* t = NULL);

    // rule of three :)
    Smart_ptr(const Smart_ptr<T, size>& obj); // copy constructor
    Smart_ptr& operator= (T* const value);
    Smart_ptr& operator= (const Smart_ptr<T, size> &rv);
    ~Smart_ptr(); // destructor;

    // make these statements(operators) deleted so that you cannot call them from the user program
    Smart_ptr<T, size> operator++() = delete;    // prefix ++
    Smart_ptr<T, size> operator--() = delete;    // prefix --
    Smart_ptr<T, size> operator++(int) = delete; // postfix ++
    Smart_ptr<T, size> operator--(int) = delete; // postfix --
    Smart_ptr<T, size> operator+ (int n) = delete;
    Smart_ptr<T, size> operator- (int n) = delete;
    bool operator!= (const Smart_ptr<T, size>& s) = delete;
    bool operator>= (const Smart_ptr<T, size>& s) = delete;
    bool operator<= (const Smart_ptr<T, size>& s) = delete;
    bool operator== (const Smart_ptr<T, size>& s) = delete;
    bool operator< (const Smart_ptr<T, size>& s) = delete;
    bool operator> (const Smart_ptr<T, size>& s) = delete;

    // redefined operators
    T& operator* ();
    T* operator->();
    T& operator[](int i);
    
    // iterator
    Smart_Iterator<T> begin();
    Smart_Iterator<T> end();

    // function that collect our garbage (it scans gclist<Info<T>>)
    bool garbagecollect();
        
    T* getaddr() const;
    int getlength() const;
    void set_gclist_collect_size(unsigned _size);
    void showlist();
};

// out class is templated, so we should define this
template <class T, int size> 
std::list<Info<T> > Smart_ptr<T, size>::gclist;

template <class T, int size>
void Smart_ptr<T, size>::add_to_gclist(T* data) {
    Info<T> obj(data, size);
    if (gclist.size() > gclist_collect_size) {
        garbagecollect();
    }
    gclist.push_front(obj);
}

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
    if (p != gclist.end()) {
        p->increfcount();
    }
    else {
        add_to_gclist(t);
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
Smart_ptr<T, size>& Smart_ptr<T, size>::operator= (T* const value) {
    typename std::list<Info<T> >::iterator p;
    p = findInfoInList(this->addr);
    if (p != gclist.end()) {
        p->decrefcount();
        std::cout << "old qref:" << p->getrefcount() << " ";
        p = findInfoInList(value);
        std::cout << "new qref:" << p->getrefcount() << " ";
        if (p != gclist.end()) {
            p->increfcount();
            addr = value;
        }
        else {
            add_to_gclist(value);
        }
    }
    else {
        throw Smart_gc_exception("failed to assigned(operator=)");
    }
}

template <class T, int size>
Smart_ptr<T, size>& Smart_ptr<T, size>::operator= (const Smart_ptr<T, size>& obj) {
    typename std::list<Info<T> >::iterator p;
    p = findInfoInList(this->addr);
    if (p != gclist.end()) {
        p->decrefcount();
        p = findInfoInList(obj.getaddr());
        if (p != gclist.end()) {
            p->increfcount();
            addr = obj.getaddr();
        }
        else {
            throw Smart_gc_exception("failed to assigned(operator=)");
        }
    }
    else {
        throw Smart_gc_exception("failed to assigned(operator=)");
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
    if (i >= 0 && i <= length)
        return addr[i];
    else
        throw Smart_gc_exception("Out of range [i]");
}

template <class T, int size>
Smart_Iterator<T> Smart_ptr<T,size>::begin() {
    int _size;
    if (is_array) { 
        _size = length;
    }
    else {
        _size = 1;
    }
    return Smart_Iterator<T>(addr, addr, addr + _size);
}

template <class T, int size>
Smart_Iterator<T> Smart_ptr<T,size>::end() {
    int _size;
    if (is_array) { 
        _size = length;
    }
    else {
        _size = 1;
    }
    return Smart_Iterator<T>(addr + _size, addr, addr + _size);
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
            
            gclist.remove(*p); 
            if(p->getallocmem()) {
                if (p->getisarray()) {
                    delete[] p->getallocmem();
                    //std::cout << std::endl <<this->gclist.size() << std::endl;

                }
                else {
                    delete p->getallocmem();   
                    std::cout << std::endl <<this->gclist.size() << std::endl;
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

template <class T, int size>
void Smart_ptr<T, size>::set_gclist_collect_size(unsigned _size) {
    gclist_collect_size = _size;
}


template <class T, int size> 
void Smart_ptr<T, size>::showlist() {  
    typename std::list<Info<T> >::iterator p;
    std::cout << "gclist<" << typeid(T).name() << ", "  << size << ">:\n";  
    std::cout << "memPtr      refcount    value\n";  
    if(gclist.begin() == gclist.end()) {  
        std::cout << "           -- Empty --\n\n";  
        return;  
    }
    for(p = gclist.begin(); p != gclist.end(); p++) {  
        std::cout <<  "[" << (void *)p->getallocmem() << "]"<< "      " << p->getrefcount() << "     ";  
        if(p->getallocmem()) std::cout << "   " << *p->getallocmem(); 
        else std::cout << "   ---";  
        std::cout << std::endl;        
  }  
  std::cout << std::endl;  
}  

#endif
