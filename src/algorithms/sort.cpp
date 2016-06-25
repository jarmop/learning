#include <iostream>
#include <vector>
#include <list>

using namespace std;

void printData(vector<int> &data) {
    for (int i = 0; i < data.size(); i++) {
        cout << data[i];
    }
    cout << endl;
}

/**
 * 10k: 1.2 s, 20k: ~4.38 s, 30k: ~9 s, 40k: ~17 s => O(4n)
 */
void bubbleSort(vector<int> &data) {
    int swapCount = 0;
    for (int i = 1; i < data.size(); i++) {
//        cout << "i: " << data[i] << ", i-1: " << data[i-1] << endl;
        if (data[i] > data[i - 1]) {
            int temp = data[i - 1];
            data[i - 1] = data[i];
            data[i] = temp;
            swapCount++;
        }
    }
    if (swapCount > 0) {
//        cout << "Do it again!" << endl;
        bubbleSort(data);
    }
}

/**
 * 10k: 1.6 s, 20k: ~4.45 s, 30k: ~10 s, 40k: 25 s => O(5n)
 * Lost to bubble sort :(
 */
void jarmoSort(vector<int> &data) {
    list<int> sortedData;
    sortedData.push_back(data[0]);
    for (int i = 1; i < data.size(); i++) {
        list<int>::const_iterator it = sortedData.begin();
//        cout << "data[i]: " << data[i] << endl;
        while (it != sortedData.end()) {
            if (data[i] > *it) {
//                cout << "Replace *it: " << *it << endl;
                sortedData.insert(it, data[i]);
                break;
            }
            it++;
            if (it == sortedData.end()) {
//                cout << "push back" << endl;
                sortedData.push_back(data[i]);
            }
        }
    }
    list<int>::const_iterator it = sortedData.begin();
//    cout << "dafuk" << endl;
    for (int i = 0; i < data.size(); i++) {
//        cout << "*it" << *it << endl;
        data[i] = *it;
        it++;
    }
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

int main() {
    // Bubble sort
    vector<int> data;
    for (int i = 0; i < 40000; i++) {
        data.push_back(rand() % 10);
    }
    vector<int> data2 = data;

//    printData(data);

    startTimer();

    bubbleSort(data);

    stopTimer();

//    printData(data);

//    printData(data2);

    startTimer();

    jarmoSort(data2);

    stopTimer();

//    printData(data2);

    // testaa omalla algoritmilla meneekö nopeammin isolla joukolla


    // Merge sort

    // Quick sort

    cout << "test";
}