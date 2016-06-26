#include <iostream>
#include <vector>
#include <list>

#include "SortAlgorithm.h"

using namespace std;

void printData(vector<int> &data) {
    for (int i = 0; i < data.size(); i++) {
        cout << data[i];
    }
    cout << endl;
}

clock_t startTime;
clock_t endTime;

void startTimer() {
    startTime = clock();
}

void stopTimer() {
    endTime = clock();
    double elapsed_secs = double(endTime - startTime) / CLOCKS_PER_SEC;
    cout << "Time passed: " << elapsed_secs << " s" << endl;
}

vector<int> data, originalData;
void initializeTestData() {
    if (originalData.size() == 0) {
        for (int i = 0; i < 10000; i++) {
            originalData.push_back(rand() % 10);
        }
    }
    data = originalData;
}


void runTest(SortAlgorithm &algorithm) {
    initializeTestData();
    startTimer();
    algorithm.sort(data);
    stopTimer();
}

int main() {
    char selectedAlgorithm;

    cout << "Select algorithm (default is C):" << endl;
    cout << "[C]ustom [B]ubble" << endl;
    cin >> selectedAlgorithm;

//    cout << selectedAlgorithm << endl;


    SortAlgorithm* sortAlgorithm;
    switch (selectedAlgorithm) {
        case 'B':
        case 'b':
            cout << "Selected B" << endl;
            sortAlgorithm = new BubbleSort;
            break;
        case 'C':
        case 'c':
        default :
            cout << "Selected C" << endl;
            sortAlgorithm = new CustomSort;
    }

    runTest(*sortAlgorithm);


    return 0;

    // Bubble sort
    vector<int> data;
    for (int i = 0; i < 40000; i++) {
        data.push_back(rand() % 10);
    }
    vector<int> data2 = data;

//    printData(data);

    startTimer();

//    bubbleSort(data);

    stopTimer();

//    printData(data);

//    printData(data2);

    startTimer();

//    jarmoSort(data2);

    stopTimer();

//    printData(data2);

    // testaa omalla algoritmilla meneekö nopeammin isolla joukolla


    // Merge sort

    // Quick sort

    cout << "test";
}