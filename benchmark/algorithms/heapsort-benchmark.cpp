#include <iostream>

#include "benchmark/benchmark.h"

#include "HeapSort.h"

using namespace std;

int minSize = 1000;
int maxSize = 1000<<3;
vector<int> getTestData(int size) {
    vector<int> data;
    srand(time(0));
    for (int i = 0; i < size; i++) {
        data.push_back(rand() % size);
    }
    return data;
}

static void BM_MakeHeapStl(benchmark::State &state) {
    vector<int> data = getTestData(state.range_x());
    while (state.KeepRunning()) {
        make_heap(data.begin(), data.end());
    }
//    state.SetComplexityN(state.range_x());
}
//BENCHMARK(BM_MakeHeapStl)->Range(minSize, maxSize)->Complexity();
BENCHMARK(BM_MakeHeapStl)->Range(minSize, maxSize);

static void BM_MakeHeapCustom(benchmark::State& state) {
    vector<int> data = getTestData(state.range_x());
    HeapSort heapSort;
    while (state.KeepRunning()) {
        heapSort.sort(data);
    }
//    state.SetComplexityN(state.range_x());
}
//BENCHMARK(BM_MakeHeapCustom)->Range(minSize, maxSize)->Complexity();
BENCHMARK(BM_MakeHeapCustom)->Range(minSize, maxSize);

BENCHMARK_MAIN();