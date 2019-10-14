#ifndef __INFO
#define __INFO

template <class T> class Info {
    unsigned refcount;
    T* allocmem;
    unsigned size;
    bool is_array;

public:

    Info() = delete;
    Info(T* mem, unsigned size = 0); 
    bool operator== (const Info<T>& s); // specially to search in STL list

    unsigned getsize() const;
    T* getallocmem() const;
    void decrefcount();
    void increfcount();
    int getrefcount() const;
    int getisarray() const; 
};

template <class T>
Info<T>::Info(T* mem, unsigned size) {
    this->refcount = 0;
    this->allocmem = mem;
    this->size = size;
    if (size > 0)
        is_array = true;
    else 
        is_array = false;
}

template <class T>
bool Info<T>::operator== (const Info<T>& s) {
    return allocmem == s.getallocmem();
}

template <class T>
unsigned Info<T>::getsize() const{
    return size;
}

template <class T>
T* Info<T>::getallocmem() const{
    return allocmem;
}

template <class T>
void Info<T>::decrefcount() {
    refcount--;
}

template <class T>
void Info<T>::increfcount() {
    refcount++;
}

template <class T>
int Info<T>::getrefcount() const {
    return refcount;
}

template <class T>
int Info<T>::getisarray() const {
    return is_array;
}

#endif