
#include "include/gc.hpp"
#include <iostream>
#include <new>

int function() {
      Smart_ptr<int> A = new int;
}

int main() {
  function();
}