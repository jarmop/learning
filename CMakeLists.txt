cmake_minimum_required(VERSION 3.5)
project(data-structures-and-algorithms)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

#include_directories(library/googletest/googletest/include)

add_subdirectory(src/data-structures)
add_subdirectory(src/algorithms)

