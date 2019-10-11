#include "include/info.hpp"


template <class T>
Info<T>::Info(T* mem, unsigned size) {
    this->refcount = 0;
    this->allocmem = mem;
    this->size = size;
}

template <class T>
bool Info<T>::compare(T* mem) {
    return mem == allocmem;
}

template <class T>
bool Info<T>::operator== (const Info<T>& s) {
    return s.compare(allocmem);
}

