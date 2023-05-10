cmake_minimum_required(VERSION 3.21)
project(lab1)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(lab1
        ScapegoatTree.cpp
        ScapegoatTree.h
        AvlTree.cpp
        AvlTree.h
        mymain.cpp)