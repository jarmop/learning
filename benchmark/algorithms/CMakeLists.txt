set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/src/algorithms)

include_directories(${CMAKE_SOURCE_DIR}/library/benchmark/include ${SOURCE_DIR})

add_executable(asymptotic-complexity asymptotic-complexity.cpp ${SOURCE_DIR}/SortAlgorithm.cpp ${SOURCE_DIR}/SortAlgorithm.h)
target_link_libraries(asymptotic-complexity ${CMAKE_SOURCE_DIR}/library/benchmark/build/src/libbenchmark.a)

add_executable(heapsort-benchmark heapsort-benchmark.cpp ${SOURCE_DIR}/HeapSort.cpp ${SOURCE_DIR}/HeapSort.h)
target_link_libraries(heapsort-benchmark ${CMAKE_SOURCE_DIR}/library/benchmark/build/src/libbenchmark.a)