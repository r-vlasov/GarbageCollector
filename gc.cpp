#include "include/gc.hpp"
#include "include/info.hpp"
#include <list>


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
typename std::list<Info<T> >::iterator Smart_ptr<T, size>::findInfoInList(T* t){
    typename std::list<Info<T> >::std::iterator p;
    for (p = gclist.begin(); p != gclist.end(); p++) {
        if (p->getallomem() == t)
            return p;
    }
    return p;
}

template <class T, int size> 
Smart_ptr<T, size>::Smart_ptr(T* t) {
    typename std::list<Info<T> >::std::iterator p;
    p = findInfoInList(t);
    if (p != gclist.end())
        p->refcount++;
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
    p = findInfoInList(obj.addr);
    p->refcount++;
    addr = obj.getaddr();
    length = obj.getlength();
    if (length > 0)
        is_array = true;
    else
        is_array = false;
}

template <class T, int size>
Smart_ptr<T, size>::~Smart_ptr() {
    typename std::list<Info<T> >::std::iterator p;
    p = findInfoInList(addr);
    if (p->refcount)
        p->refcount--;
    garbagecollect();
}

template <class T, int size>
bool Smart_ptr<T, size>::garbagecollect() {
    typename std::list<Info<T> >::std::iterator p;
    bool freed = false;
    do {
        for (p = gclist.begin(); p != gclist.end(); p++) {
            if(p->refcount > 0)
                continue;
            freed = true;

            gclist.remove(*p); 
            if(p->getallocmem()) {
                if (p->is_array) {
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
