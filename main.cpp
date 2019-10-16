
#include "include/gc.hpp"
/*#include <iostream>
#include <new>

int function() {
    try {
        Smart_ptr<int> a;
        Smart_ptr<int> b = new int;
        Smart_ptr<int> c(a);
        Smart_ptr<int> d = new int;
        Smart_ptr<int> f(d);
    }
    catch (Smart_iter_exception& exc) {
        std::cout << "1";
    }
    catch (Smart_gc_exception& exc) {
        std::cout << "2";
    }
}

int main() { 
    Smart_ptr<int> b = new int;
    Smart_ptr<int> c;
    function();
    
    std::cout << "mark\n";
}

*/
#include <cmath>
using namespace std;
// A simple class for load testing GCPtr.
// Create an inserter for LoadTest.
int main() {
  Smart_ptr<int> mp;
  int i;
  for(i = 1; i <= 2000; i++) {
     // std::cout << i << std::endl;
      mp = new int;
      int c = pow(2.0, 5.2);
      if(!(i%100)) {
        cout << "gclist contains " << mp.getgclistsize() << " entries.\n" << std::endl;
  }}
  return 0;
}
