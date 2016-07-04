set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/src/algorithms)

include_directories(${CMAKE_SOURCE_DIR}/library/benchmark/include ${SOURCE_DIR})

add_executable(asymptotic-complexity asymptotic-complexity.cpp ${SOURCE_DIR}/SortAlgorithm.cpp ${SOURCE_DIR}/SortAlgorithm.h)
target_link_libraries(asymptotic-complexity ${CMAKE_SOURCE_DIR}/library/benchmark/build/src/libbenchmark.a)