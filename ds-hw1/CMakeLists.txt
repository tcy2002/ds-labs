cmake_minimum_required(VERSION 3.21)
project(hw4)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(hw4
        Container.h
        SkipList.cpp
        SkipList.h
		AVLTree.cpp
		AVLTree.h
		main.cpp)
