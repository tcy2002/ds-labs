cmake_minimum_required(VERSION 3.21)
project(ds-lab2)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(ds-lab2 FixedSP.h FixedSP.cpp mymain.cpp ThreadPool.h)