add_executable(sort sort.cpp SortAlgorithm.cpp SortAlgorithm.h)

include_directories(${CMAKE_SOURCE_DIR}/library/benchmark/include)
add_executable(benchmark benchmark.cpp SortAlgorithm.cpp SortAlgorithm.h)
target_link_libraries(benchmark ${CMAKE_SOURCE_DIR}/library/benchmark/build/src/libbenchmark.a)