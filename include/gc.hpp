#ifndef __GC
#define __GC

#include <list>
#include <stddef.h>
#include "iterator.hpp"
#include "exception.hpp"
#include "info.hpp"
#include <iostream>
#include <pthread.h>
#include <chrono>
#include <thread>

#define MAGIC_SIZE 10
#define WAITINGTIME 1 // ms

template <class T, int size = 0>
class Smart_ptr {
    static std::list<Info<T> > gclist;
    static unsigned int count;
    static bool mutex_init;
    static bool thread_init;
    static pthread_t threadid;
    static pthread_mutex_t mutex;
    
    T* addr;
    unsigned length; // if it is a array -> size of array, else size = 0;
    union {
        bool is_array;
        bool check;
    };

    // we will run gc if current time - lasttime_gc >= 1ms
    static std::chrono::high_resolution_clock::time_point lasttime_gc;

    // the limit size after which the garbagecollect will be called when trying to increase the size of the gclist 
    unsigned gclist_collect_size = MAGIC_SIZE;

    // function that add Info object at gclist
    void add_to_gclist(T* data);
    
    // iterator on ptr in list
    typename std::list<Info<T> >::iterator findInfoInList(T* ptr);

    // gc function
    static void* (*gc) (void*);

public:
    Smart_ptr(T* t = NULL);

    // rule of three :)
    Smart_ptr(const Smart_ptr<T, size>& obj); // copy constructor
    Smart_ptr& operator= (T* const value);
    Smart_ptr& operator= (const Smart_ptr<T, size> &rv);
    ~Smart_ptr(); // destructor;

    // make these statements(operators) deleted so that you cannot call them from the user program
    Smart_ptr<T, size> operator++() = delete;    // prefix ++
    Smart_ptr<T, size> operator--() = delete;    // prefix --
    Smart_ptr<T, size> operator++(int) = delete; // postfix ++
    Smart_ptr<T, size> operator--(int) = delete; // postfix --
    Smart_ptr<T, size> operator+ (int n) = delete;
    Smart_ptr<T, size> operator- (int n) = delete;
    bool operator!= (const Smart_ptr<T, size>& s) = delete;
    bool operator>= (const Smart_ptr<T, size>& s) = delete;
    bool operator<= (const Smart_ptr<T, size>& s) = delete;
    bool operator== (const Smart_ptr<T, size>& s) = delete;
    bool operator< (const Smart_ptr<T, size>& s) = delete;
    bool operator> (const Smart_ptr<T, size>& s) = delete;

    // redefined operators
    T& operator* ();
    T* operator->();
    T& operator[](int i);
    
    // iterator
    Smart_Iterator<T> begin();
    Smart_Iterator<T> end();

    // general iterator
    Smart_Iterator<T> iterator;

    // small functions
    T* getaddr() const;
    int getlength() const;
    int getgclistsize() const;
    void set_gclist_collect_size(unsigned _size);

    // multitasking gc
    bool running();;

    // function that collect our garbage (it scans gclist<Info<T>>)
    bool garbagecollect();
};

// template function //
// we will use pointer on this function :3 //
template <class T, int size = 0 >
void* thread_gc(void* params) {
    Smart_ptr<T, size>* obj = reinterpret_cast<Smart_ptr<T, size>*>(params);
    std::cout << "GC is running" << std::endl;
    while (obj->getgclistsize()) {
        obj->garbagecollect();
    }
    std::cout << "GC is dead" << std::endl;
}

// out class is templated, so we should define this
template <class T, int size> 
std::list<Info<T> > Smart_ptr<T, size>::gclist;

template <class T, int size>
unsigned int Smart_ptr<T, size>::count = 0;

template <class T, int size>
bool Smart_ptr<T, size>::thread_init = false;

template <class T, int size>
bool Smart_ptr<T, size>::mutex_init = false;

template <class T, int size>
pthread_t Smart_ptr<T, size>::threadid;

template <class T, int size>
pthread_mutex_t Smart_ptr<T, size>::mutex;

template <class T, int size>
std::chrono::high_resolution_clock::time_point Smart_ptr<T, size>::lasttime_gc = 
                    std::chrono::high_resolution_clock::now();

template <class T, int size>
void* (*Smart_ptr<T, size>::gc)(void*) = &thread_gc<T,size> ;


template <class T, int size>
void Smart_ptr<T, size>::add_to_gclist(T* data) {
    Info<T> obj(data, size);
    gclist.push_front(obj);
}

template <class T, int size>
typename std::list<Info<T> >::iterator Smart_ptr<T, size>::findInfoInList(T* t){
    typename std::list<Info<T> >::iterator p;
    for (p = gclist.begin(); p != gclist.end(); p++) {
        if (p->getallocmem() == t)
            return p;
    }
    return p;
}

template <class T, int size> 
Smart_ptr<T, size>::Smart_ptr(T* t) {
    typename std::list<Info<T> >::iterator p;
    
    if (!mutex_init) {
        // mutex init
        pthread_mutex_init(&mutex, NULL);
        mutex_init = true;
        pthread_mutex_lock(&mutex);
    }
    p = findInfoInList(t);
    if (p != gclist.end()) {
        p->increfcount();
    }
    else {
        add_to_gclist(t);
    }
    addr = t;
    length = size;
    count++;
    if (!thread_init) {
        thread_init = true;
        // get the default attributes flow of execution
        pthread_attr_t attr;
        pthread_attr_init(&attr);
        // run thread
        pthread_create(&threadid, &attr, gc, (void*)this);
    }
    if (size > 0) 
        is_array = true;
    else 
        is_array = false;
    pthread_mutex_unlock(&mutex);
}

template <class T, int size>
Smart_ptr<T, size>::Smart_ptr(const Smart_ptr<T, size>& obj) {
    pthread_mutex_lock(&mutex);
    typename std::list<Info<T> >::iterator p;
    p = findInfoInList(obj.getaddr());
    p->increfcount();
    addr = obj.getaddr();
    length = obj.getlength();
    if (length > 0)
        is_array = true;
    else
        is_array = false;
    count++;
    pthread_mutex_unlock(&mutex);
}

template <class T, int size>
Smart_ptr<T, size>& Smart_ptr<T, size>::operator= (T* const value) {
    pthread_mutex_lock(&mutex);
    typename std::list<Info<T> >::iterator p;
    p = findInfoInList(this->addr);
    if (p != gclist.end()) { 
        p->decrefcount();
        p = findInfoInList(value);
        if (p != gclist.end()) {
            p->increfcount();
        }
        else {
            add_to_gclist(value);
        }
        addr = value;
    }
    else {
        throw Smart_gc_exception("failed to assigned(operator=)");
    }
    pthread_mutex_unlock(&mutex);
}

template <class T, int size>
Smart_ptr<T, size>& Smart_ptr<T, size>::operator= (const Smart_ptr<T, size>& obj) {
    pthread_mutex_lock(&mutex);
    typename std::list<Info<T> >::iterator p;
    p = findInfoInList(this->addr);
    if (p != gclist.end()) {
        p->decrefcount();
        p = findInfoInList(obj.getaddr());
        if (p != gclist.end()) {
            p->increfcount();
            addr = obj.getaddr();
        }
        else {
            throw Smart_gc_exception("failed to assigned(operator=)");
        }
    }
    else {
        throw Smart_gc_exception("failed to assigned(operator=)");
    }
    pthread_mutex_unlock(&mutex);
}

template <class T, int size>
Smart_ptr<T, size>::~Smart_ptr() {
    typename std::list<Info<T> >::iterator p;
    p = findInfoInList(addr);
    if (p->getrefcount()) {
        p->decrefcount();
          
    }
    count--; 
    if (count == 0) {
        pthread_join(threadid, NULL);
        thread_init = false;
    }
}

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
    if (i >= 0 && i <= length)
        return addr[i];
    else
        throw Smart_gc_exception("Out of range [i]");
}

template <class T, int size>
Smart_Iterator<T> Smart_ptr<T,size>::begin() {
    int _size;
    if (is_array) { 
        _size = length;
    }
    else {
        _size = 1;
    }
    return Smart_Iterator<T>(addr, addr, addr + _size);
}

template <class T, int size>
Smart_Iterator<T> Smart_ptr<T,size>::end() {
    int _size;
    if (is_array) { 
        _size = length;
    }
    else {
        _size = 1;
    }
    return Smart_Iterator<T>(addr + _size, addr, addr + _size);
}

template <class T, int size>
bool Smart_ptr<T, size>::running() {
    return (bool)count;
}
#include <unistd.h>
template <class T, int size>
bool Smart_ptr<T, size>::garbagecollect() {    
    typename std::list<Info<T> >::iterator p;
    bool freed = false;
    pthread_mutex_lock(&mutex);
    std::chrono::high_resolution_clock::time_point curtime = std::chrono::high_resolution_clock::now();
    auto _ms = std::chrono::duration_cast<std::chrono::milliseconds> ( curtime - lasttime_gc );
    if (_ms.count() < WAITINGTIME) {
        pthread_mutex_unlock(&mutex);
        std::this_thread::sleep_for(std::chrono::milliseconds(WAITINGTIME));
        lasttime_gc = curtime;
        return freed;
    }

    int before = gclist.size();
    do {
        for (p = gclist.begin(); p != gclist.end(); p++) {
            if(p->getrefcount() > 0)
                continue;
            freed = true;
            
            gclist.remove(*p); 
            if(p->getallocmem()) {
                if (p->getisarray()) {
                    delete[] p->getallocmem();
                    //std::cout << std::endl <<this->gclist.size() << std::endl;

                }
                else {
                    delete p->getallocmem();   
                    //std::cout << std::endl <<this->gclist.size() << std::endl;
                 }
            }
            break;
        }
    } while (p!= gclist.end());
    int after = gclist.size();
    pthread_mutex_unlock(&mutex);
    return freed;
}

template <class T, int size>
T* Smart_ptr<T, size>::getaddr() const {
    return addr;
}

template <class T, int size>
int Smart_ptr<T, size>::getlength() const {
    return length;
}

template <class T, int size>
int Smart_ptr<T, size>::getgclistsize() const {
    return gclist.size();
}

template <class T, int size>
void Smart_ptr<T, size>::set_gclist_collect_size(unsigned _size) {
    gclist_collect_size = _size;
}

#endif
