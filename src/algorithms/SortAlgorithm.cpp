//
// Created by Jarmo Pihlajaniemi on 26/06/16.
//

#include "SortAlgorithm.h"

void BubbleSort::sort(vector<int> &data) {
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
        this->sort(data);
    }
}

void CustomSort::sort(vector<int> &data) {
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
