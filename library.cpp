#include "library.h"

#include <iostream>

template<class T>
A<T>::A(T val) {
    this->value = val;
}

template
class A<int>;

extern "C" {
B *bFactory() {
    return new B(0);
}
void sayB(B *b) {
    std::cout << " Say B:" << b->value << std::endl;
}
void deleteB(B *b) {
    delete b;
}
void sayHelloWorld() {
    std::cout << "hello world";
}
}

