#include <iostream>

#include "benchmark/benchmark.h"

#include "SortAlgorithm.h"

using namespace std;

int minSize = 1000;
int maxSize = 1000<<3;
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
BENCHMARK(BM_MergeSort)->Range(minSize, maxSize);

static void BM_BubbleSort(benchmark::State& state) {
    vector<int> data = getTestData(state.range_x());
    SortAlgorithm* sortAlgorithm = new BubbleSort;
    while (state.KeepRunning()) {
        sortAlgorithm->sort(data);
    }
}
BENCHMARK(BM_BubbleSort)->Range(minSize, maxSize);

static void BM_CustomSort(benchmark::State& state) {
    vector<int> data = getTestData(state.range_x());
    SortAlgorithm* sortAlgorithm = new CustomSort;
    while (state.KeepRunning()) {
        sortAlgorithm->sort(data);
    }
}
BENCHMARK(BM_CustomSort)->Range(minSize, maxSize);

BENCHMARK_MAIN();