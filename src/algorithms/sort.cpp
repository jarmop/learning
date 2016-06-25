#include <iostream>
#include <vector>

using namespace std;

void printData(vector<int> &data) {
    for (int i = 0; i < data.size(); i++) {
        cout << data[i];
    }
    cout << endl;
}

void bubbleSort(vector<int> &data) {
    int swapCount = 0;
    for (int i = 1; i < data.size(); i++) {
        cout << "i: " << data[i] << ", i-1: " << data[i-1] << endl;
        if (data[i] > data[i - 1]) {
            int temp = data[i - 1];
            data[i - 1] = data[i];
            data[i] = temp;
            swapCount++;
        }
    }
    if (swapCount > 0) {
        cout << "Do it again!" << endl;
        bubbleSort(data);
    }
}

int main() {
    // Bubble sort
    vector<int> data;
    for (int i = 0; i < 10; i++) {
        data.push_back(rand() % 10);
    }

    printData(data);

    bubbleSort(data);

    printData(data);



    // Merge sort

    // Quick sort

    cout << "test";
}