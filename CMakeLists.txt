cmake_minimum_required(VERSION 3.5)
project(data-structures-and-algorithms)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(library/googletest/googletest/include src/data-structures)

set(SOURCE_FILES src/data-structures/main.cpp)
add_executable(main ${SOURCE_FILES})

target_link_libraries(main ${CMAKE_SOURCE_DIR}/library/googletest/build/googlemock/gtest/libgtest.a)