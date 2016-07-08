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
//BENCHMARK(BM_MakeHeapStl)->Range(minSize, maxSize);

static void BM_MakeHeapCustom(benchmark::State& state) {
    vector<int> data = getTestData(state.range_x());
    HeapSort heapSort;
    while (state.KeepRunning()) {
        heapSort.makeHeap(data.begin(), data.end());
    }
//    state.SetComplexityN(state.range_x());
}
//BENCHMARK(BM_MakeHeapCustom)->Range(minSize, maxSize)->Complexity();
//BENCHMARK(BM_MakeHeapCustom)->Range(minSize, maxSize);

vector<int> getTestHeap(int size) {
    vector<int> data = getTestData(size);
    make_heap(data.begin(), data.end());
    return data;
}

static void BM_SortHeapStl(benchmark::State &state) {
//    vector<int> data = getTestHeap(state.range_x());
    vector<int> data = getTestData(state.range_x());
    while (state.KeepRunning()) {
        make_heap(data.begin(), data.end());
        sort_heap(data.begin(), data.end());
    }
    state.SetComplexityN(state.range_x());
}
BENCHMARK(BM_SortHeapStl)->Range(minSize, maxSize)->Complexity(benchmark::oNLogN);
//BENCHMARK(BM_SortHeapStl)->Range(minSize, maxSize);

static void BM_SortHeapCustom(benchmark::State& state) {
//    vector<int> data = getTestHeap(state.range_x());
    vector<int> data = getTestData(state.range_x());    HeapSort heapSort;
    while (state.KeepRunning()) {
        heapSort.makeHeap(data.begin(), data.end());
        heapSort.sortHeap(data.begin(), data.end());
    }
    state.SetComplexityN(state.range_x());
}
BENCHMARK(BM_SortHeapCustom)->Range(minSize, maxSize)->Complexity(benchmark::oNSquared);
//BENCHMARK(BM_SortHeapCustom)->Range(minSize, maxSize);

BENCHMARK_MAIN();