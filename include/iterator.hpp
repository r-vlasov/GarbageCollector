#ifndef __ITERATOR
#define __ITERATOR

#include "exception.hpp"

template <class T> 
class Smart_Iterator {
    T* ptr; // current poier value;
    T* end; // pointer on element past end
    T* begin; // first pointer value;
    unsigned length;

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
    Smart_Iterator<T> operator--(int); // postfix --
    
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
        throw Smart_iter_exception("Out of range");
    return *ptr;
}

template <class T>
T* Smart_Iterator<T>::operator->() {
    if (ptr < begin || ptr >= end)
       throw Smart_iter_exception("Out of range");
    return ptr;
}

template <class T>
Smart_Iterator<T> Smart_Iterator<T>::operator++() {
    ++ptr;
    return *this;
}

template <class T>
Smart_Iterator<T> Smart_Iterator<T>::operator--() {
    --ptr;
    return *this;
}

template <class T>
Smart_Iterator<T> Smart_Iterator<T>::operator++(int) {
    T* _p = ptr;
    ++ptr;
    return Smart_Iterator<T>(_p, begin, end);
}

template <class T>
Smart_Iterator<T> Smart_Iterator<T>::operator--(int) {
    T* _p = ptr;
    --ptr;
    return Smart_Iterator<T>(_p, begin, end);
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
    return *this;
}

template <class T>
Smart_Iterator<T> Smart_Iterator<T>::operator-(int n) {
    ptr -= n;
    return *this;
}

#endif