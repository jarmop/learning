//
// Created by Jarmo Pihlajaniemi on 26/06/16.
//

#include <iostream>
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

list<int> MergeSort::mergeParts(list<int> part1, list<int> part2) {
    list<int> part;
    while (!part1.empty() && !part2.empty()) {
        if (part1.front() >= part.front()) {
            part.push_back(part1.front());
            part1.pop_front();
        } else {
            part.push_back(part2.front());
            part2.pop_front();
        }
    }
    // At least one part should be empty at this point so we can just get the rest from the other.
    while (!part1.empty()) {
        part.push_back(part1.front());
        part1.pop_front();
    }
    while (!part2.empty()) {
        part.push_back(part2.front());
        part2.pop_front();
    }
    return part;
}

vector<list<int>> MergeSort::merge(vector<list<int>> &container) {
    vector<list<int>> sortedContainer;
    for (int i = 0; i < container.size(); i+=2) {
        list<int> part = this->mergeParts(container[i], container[i + 1]);
        if (i + 2 == container.size() - 1) {
            // We are at the end of an uneven container. We must handle the leftover unit.
            this->mergeParts(part, container.back());
            break;
        }
        sortedContainer.push_back(part);
    }
    if (sortedContainer.size() > 1) {
        this->merge(sortedContainer);
    }

    return sortedContainer;
}

void MergeSort::sort(vector<int> &data) {
    vector<list<int>> container;
    for (int i = 0; i < data.size(); i+=2) {
        list<int> part;
        if (data[i] > data[i+1]) {
            part.push_back(data[i]);
            part.push_back(data[i+1]);
        } else {
            part.push_back(data[i+1]);
            part.push_back(data[i]);
        }
        if (i + 2 == data.size() - 1) {
            // We are at the end of an uneven vector. We must handle the leftover unit.
            int leftOver = data.back();
            if (leftOver > part.front()) {
                part.push_front(leftOver);
            } else if (leftOver > part.back()) {
                part.insert(part.end(), leftOver);
            } else {
                part.push_back(leftOver);
            }
            container.push_back(part);
            break;
        }
        container.push_back(part);
    }
    vector<list<int>> sortedContainer = this->merge(container);

    data.clear();
    list<int> sortedList = sortedContainer.front();
    list<int>::iterator it = sortedList.begin();
    while (it != sortedList.end()) {
        data.push_back(*it);
        it++;
    }
}