#ifndef __INFO
#define __INFO

template <class T> class Info {
    public:
    unsigned refcount;
    T* allocmem;
    unsigned size;
    bool is_array;

public:
    Info(T* mem, unsigned size = 1); 
    bool compare(T* mem);
    bool operator== (const Info<T>& s); // specially to search in STL list

    unsigned getsize();
    T* getallocmem();
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
}

template <class T>
bool Info<T>::compare(T* mem) {
    return mem == allocmem;
}

template <class T>
bool Info<T>::operator== (const Info<T>& s) {
   // return s.compare(allocmem);
    return allocmem == s.allocmem;
}

template <class T>
unsigned Info<T>::getsize() {
    return size;
}

template <class T>
T* Info<T>::getallocmem() {
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