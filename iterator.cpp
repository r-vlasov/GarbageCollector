#include "include/iterator.hpp"
#include "include/exception.hpp"
#include <stddef.h>

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
       // throw Out_of_range();
    return *ptr;
}

template <class T>
T* Smart_Iterator<T>::operator->() {
    if (ptr < begin || ptr >= end)
       // throw Out_of_range();
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