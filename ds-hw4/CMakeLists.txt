cmake_minimum_required(VERSION 3.21)
project(hw4_avl)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(hw4_avl main.cpp avlTree.h avlTree.cpp skipList.h skipList.cpp)
