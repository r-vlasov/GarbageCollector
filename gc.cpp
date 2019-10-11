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
list<Info<T> >::iterator Smart_ptr<T, size>::findInfoInList(T* t){
    list<Info<T> >::iterator p;
    for (p = gclist.begin(), p != gclist.end(), p++) {
        if (p->allocmem == t)
            return p;
    }
    return p;
}

template <class T, int size> 
Smart_ptr<T, size>::Smart_ptr(T* t = NULL) {
    list<Info<T> >::iterator p;
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
    list<Info<T> >::iterator p;
    p = findInfoInList(obj.addr);
    p->refcount++;
    addr = obj.getaddr();
    length = ob.getlength();
    if (length > 0)
        is_array = true;
    else
        is_array = false;
}





}
