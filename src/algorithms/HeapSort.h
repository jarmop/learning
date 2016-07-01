//
// Created by Jarmo Pihlajaniemi on 29/06/16.
//

#ifndef DATA_STRUCTURES_AND_ALGORITHMS_HEAPSORT_H
#define DATA_STRUCTURES_AND_ALGORITHMS_HEAPSORT_H


#include "SortAlgorithm.h"

class HeapSort : public SortAlgorithm {
public:
    int iParent(int i);
    int iLeftChild(int i);
    int iRightChild(int i);
    vector<int>::iterator getChild(vector<int>::iterator it);
    void recursiveMakeHeap(vector<int>::iterator current, vector<int>::iterator begin, vector<int>::iterator end);
    void makeHeap(vector<int>::iterator begin, vector<int>::iterator end);
    void sort(vector<int> &data);
};


#endif //DATA_STRUCTURES_AND_ALGORITHMS_HEAPSORT_H
