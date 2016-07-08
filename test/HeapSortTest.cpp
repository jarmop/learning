#include <iostream>
#include <cmath>
#include "../src/algorithms/HeapSort.h"

using namespace std;

void printData(vector<int> &data) {
    for (int i = 0; i < data.size(); i++) {
        cout << data[i] << ' ';
    }
    cout << endl;
}

int main() {
    vector<int> data;
    int size  = 10;
//    srand(time(0));
    for (int i = 0; i < size; i++) {
        data.push_back(rand() % size);
    }
    vector<int> data2 = data;

    printData(data);
//    printData(data2);

    make_heap(data.begin(), data.end());

    HeapSort* heapSort = new HeapSort;
    heapSort->makeHeap(data2.begin(), data2.end());


    cout << "STL" << endl;
    cout << "Heap: ";
    printData(data);
    cout << "isheap: " << is_heap(data.begin(), data.end()) << endl;

    sort_heap(data.begin(), data.end());
    cout << "Sorted: ";
    printData(data);

    cout << "******" << endl;

    cout << "Custom" << endl;
    cout << "Heap: ";
    printData(data2);
    cout << "isheap: " << is_heap(data2.begin(), data2.end()) << endl;

    heapSort->sortHeap(data2.begin(), data2.end());
    cout << "Sorted: ";
    printData(data2);

}