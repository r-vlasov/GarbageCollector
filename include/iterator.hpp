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


#endif