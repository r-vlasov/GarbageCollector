#ifndef __ITERATOR
#define __ITERATOR

#include "exception.hpp"

template <class T> 
class Smart_Iterator {
    T* ptr; // current pointer value;
    T* end; // pointer on element past end
    T* begin; // first pointer value;
    unsigned length;

    // function which called to checking exceptions
    Smart_Iterator<T> valid_check();
    Smart_Iterator<T> valid_create(Smart_Iterator<T>& _p);


public:
    Smart_Iterator();
    // Smart_Iterator(Smart_Iterator const& copy);
    Smart_Iterator(T* p, T* first, T* last);
    unsigned size() const;
    T& operator*(); // there will be the throw 'outofrange'
    T* operator->(); 

    Smart_Iterator<T> operator++();    // prefix ++
    Smart_Iterator<T> operator--();    // prefix --
    Smart_Iterator<T> operator++(int); // postfix ++
    Smart_Iterator<T> operator--(int); // pogstix --
    
    bool operator!= (const Smart_Iterator<T>& s);
    bool operator>= (const Smart_Iterator<T>& s);
    bool operator<= (const Smart_Iterator<T>& s);
    bool operator== (const Smart_Iterator<T>& s);
    bool operator< (const Smart_Iterator<T>& s);
    bool operator> (const Smart_Iterator<T>& s);

    Smart_Iterator<T> operator+ (int n);
    Smart_Iterator<T> operator- (int n);
};


template <class T>
Smart_Iterator<T>::Smart_Iterator() {
    ptr = end = begin = NULL;
    length = 0;
}

template <class T>
Smart_Iterator<T>::Smart_Iterator(T* p, T* first, T* last) {
    ptr = p;
    end = last;
    begin = first;
    length = last - first;

}

template <class T>
unsigned Smart_Iterator<T>::size() const {
    return length;
}

template <class T>
T& Smart_Iterator<T>::operator*() {
    if (ptr < begin || ptr >= end)
        throw Smart_iter_exception("Out of range (access)");
    return *ptr;
}

template <class T>
T* Smart_Iterator<T>::operator->() {
    if (ptr < begin || ptr >= end)
       throw Smart_iter_exception("Out of range (access)");
    return ptr;
}

template <class T>
Smart_Iterator<T> Smart_Iterator<T>::valid_check() {
    if (ptr <= end && ptr >= begin) 
        return *this;
    else
    {
        throw Smart_iter_exception("Out of range");
    }
}

template <class T>
Smart_Iterator<T> Smart_Iterator<T>::valid_create(Smart_Iterator<T>& _p) {
    if (ptr <= end && ptr >= begin) 
        return Smart_Iterator<T>(_p, begin, end);
    else
    {
        throw Smart_iter_exception("Out of range");
    }
}

template <class T>
Smart_Iterator<T> Smart_Iterator<T>::operator++() {
    ++ptr;
    return this->valid_check();
}

template <class T>
Smart_Iterator<T> Smart_Iterator<T>::operator--() {
    --ptr;
    return this->valid_check();
}

template <class T>
Smart_Iterator<T> Smart_Iterator<T>::operator++(int) {
    T* _p = ptr;
    ++ptr;
    if (ptr <= end && ptr >= begin) 
        return Smart_Iterator<T>(_p, begin, end);
    else
    {
        throw Smart_iter_exception("Out of range");
    }
}

template <class T>
Smart_Iterator<T> Smart_Iterator<T>::operator--(int) {
    T* _p = ptr;
    --ptr;
    if (ptr <= end && ptr >= begin) 
        return Smart_Iterator<T>(_p, begin, end);
    else
    {
        throw Smart_iter_exception("Out of range");
    }
}

template <class T>
bool Smart_Iterator<T>::operator!=(const Smart_Iterator<T>& s) {
    return ptr != s.ptr;   
}

template <class T>
bool Smart_Iterator<T>::operator>=(const Smart_Iterator<T>& s) {
    return ptr >= s.ptr;   
}

template <class T>
bool Smart_Iterator<T>::operator<=(const Smart_Iterator<T>& s) {
    return ptr <= s.ptr;   
}

template <class T>
bool Smart_Iterator<T>::operator==(const Smart_Iterator<T>& s) {
    return ptr == s.ptr;   
}

template <class T>
bool Smart_Iterator<T>::operator>(const Smart_Iterator<T>& s) {
    return ptr > s.ptr;   
}

template <class T>
bool Smart_Iterator<T>::operator<(const Smart_Iterator<T>& s) {
    return ptr < s.ptr;   
}

template <class T>
Smart_Iterator<T> Smart_Iterator<T>::operator+(int n) {
    ptr += n;
    return valid_check();
}

template <class T>
Smart_Iterator<T> Smart_Iterator<T>::operator-(int n) {
    ptr -= n;
    return valid_check();
}

#endif