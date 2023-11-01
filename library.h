#ifndef LIB_REF_LIBRARY_H
#define LIB_REF_LIBRARY_H

template<class T>
class A {
public:
    T value;
    A(T val);
};

extern template class A<int>;

class B: public A<int>{
public:
    B(int i):A(i){};
};

//void sayHelloWorld();

#endif //LIB_REF_LIBRARY_H
