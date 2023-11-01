cmake_minimum_required(VERSION 3.26)
project(sayhello)

set(CMAKE_CXX_STANDARD 17)

add_library(sayhello SHARED hello.cpp library.cpp
)
