include_directories(${CMAKE_SOURCE_DIR}/library/googletest/googletest/include)
add_executable(main main.cpp LinkedList.cpp LinkedList.hpp Node.hpp Queue.cpp Queue.hpp Stack.cpp Stack.hpp)
target_link_libraries(main ${CMAKE_SOURCE_DIR}/library/googletest/build/googlemock/gtest/libgtest.a)