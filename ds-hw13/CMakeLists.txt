cmake_minimum_required(VERSION 3.22)
project(ds_hw13)

set(CMAKE_CXX_STANDARD 14)

add_library(hw13lib cuckoo.h cuckoo.cpp)
add_executable(ds_hw13 main.cpp)
target_link_libraries(ds_hw13 hw13lib)
