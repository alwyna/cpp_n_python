#include "hello.h"
#include <bits/stdc++.h>
#include <iostream>

using namespace std;


bool hello(string *name) {
    std::cout << "Hello, " + *name << std::endl;
    return true;
}


extern "C" {
bool Hello(const char *name) {
    string nameStr = name;
    std::cout << "You passed: " + nameStr << std::endl;
    return hello(&nameStr);

}
}