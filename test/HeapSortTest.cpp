#include <iostream>
#include <cmath>
#include "../src/algorithms/HeapSort.h"

using namespace std;

void printData(vector<int> &data) {
    for (int i = 0; i < 10; i++) {
        cout << data[i] << ' ';
    }
    cout << endl;
}

int main() {
    vector<int> data;
    srand(time(0));
    for (int i = 0; i < 10; i++) {
        data.push_back(rand() % 10);
    }
    vector<int> data2 = data;

    printData(data);
//    printData(data2);

    make_heap(data.begin(), data.end());

    HeapSort* heapSort = new HeapSort;
    heapSort->sort(data2);



    printData(data);
    printData(data2);
}