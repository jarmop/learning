#include <iostream>

#include "benchmark/benchmark.h"

#include "SortAlgorithm.h"

using namespace std;

vector<int> getTestData(int size) {
    vector<int> data;
    srand(time(0));
    for (int i = 0; i < size; i++) {
        data.push_back(rand() % 10);
    }
    return data;
}

static void BM_MergeSort(benchmark::State& state) {
    vector<int> data = getTestData(state.range_x());
    SortAlgorithm* mergeSort = new MergeSort;
    while (state.KeepRunning()) {
        mergeSort->sort(data);
    }
}
BENCHMARK(BM_MergeSort)->Range(1000, 10000);

static void BM_BubbleSort(benchmark::State& state) {
    vector<int> data = getTestData(state.range_x());
    SortAlgorithm* sortAlgorithm = new BubbleSort;
    while (state.KeepRunning()) {
        sortAlgorithm->sort(data);
    }
}
BENCHMARK(BM_BubbleSort)->Range(1000, 10000);

static void BM_CustomSort(benchmark::State& state) {
    vector<int> data = getTestData(state.range_x());
    SortAlgorithm* sortAlgorithm = new CustomSort;
    while (state.KeepRunning()) {
        sortAlgorithm->sort(data);
    }
}
BENCHMARK(BM_CustomSort)->Range(1000, 10000);

BENCHMARK_MAIN();