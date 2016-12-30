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
//        srand(time(0));
        for (int i = 0; i < 40000; i++) {
            originalData.push_back(rand() % 10);
        }
    }
    data = originalData;
}


void runTest(SortAlgorithm &algorithm) {
    initializeTestData();
//    printData(data);
    startTimer();
    algorithm.sort(data);
    stopTimer();
    //    printData(data);

}

SortAlgorithm* sortAlgorithm;

void askForSortAlgorithm() {
    char selectedAlgorithm;

    cout << "Select algorithm (default is C):" << endl;
    cout << "[C]ustom [B]ubble [M]erge" << endl;
    cin >> selectedAlgorithm;

    switch (selectedAlgorithm) {
        case 'B':
        case 'b':
            cout << "Selected B" << endl;
            sortAlgorithm = new BubbleSort;
            break;
        case 'C':
        case 'c':
            cout << "Selected C" << endl;
            sortAlgorithm = new CustomSort;
        case 'M':
        case 'm':
        default :
            cout << "Selected M" << endl;
            sortAlgorithm = new MergeSort;
    }
}

int main() {
    askForSortAlgorithm();
//    sortAlgorithm = new MergeSort;
    runTest(*sortAlgorithm);

    return 0;
}