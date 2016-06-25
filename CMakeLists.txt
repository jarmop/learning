cmake_minimum_required(VERSION 3.5)
project(data-structures-and-algorithms)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(library/googletest/googletest/include)


set(SOURCE_FILES src/data-structures/main.cpp)
add_executable(main ${SOURCE_FILES})

#include_directories(src/data-structures library/googletest/googletest/include)
#link_directories(library/googletest/build/googlemock/gtest)
#target_link_libraries(main libgtest.a)


#enable_testing()
#find_package(GTest REQUIRED)
#add_executable(foo foo.cc)
target_link_libraries(main libgtest.a libgtest_main.a)
#add_test(AllTestsInFoo main)