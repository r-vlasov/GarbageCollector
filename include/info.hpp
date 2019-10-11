#ifndef __INFO
#define __INFO

template <class T> class Info {
    unsigned refcount;
    T* allocmem;
    unsigned size;

    Info(T* mem, unsigned size = 1); 
    
    bool compare(T* mem);
    bool operator== (const Info<T>& s); // specially to search in STL list
};

#endif