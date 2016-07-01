//
// Created by Jarmo Pihlajaniemi on 29/06/16.
//

#include <iostream>
#include "HeapSort.h"

int HeapSort::iParent(int i) {
    return (i - 1) / 2;
}

int HeapSort::iLeftChild(int i) {
    return i + 1;
}

int HeapSort::iRightChild(int i) {
    return i + 2;
}

//vector<int>::iterator HeapSort::getChild(vector<int>::iterator it) {
//    vector<int>::iterator leftChild = this->getChild(it + this->iLeftChild(0));
//    vector<int>::iterator rightChild = this->getChild(it + this->iRightChild(0));
//    vector<int>::iterator greatestChild = *leftChild > *rightChild ? leftChild : rightChild;
//    if (*greatestChild > *it) {
//        int tempIt = *it;
//        *it = *greatestChild;
//        *greatestChild = tempIt;
//    }
//}

void HeapSort::recursiveMakeHeap(vector<int>::iterator current, vector<int>::iterator begin, vector<int>::iterator end) {
    int currentIndex = current - begin;
    if (current < end) {
//        cout << currentIndex << endl;

        vector<int>::iterator leftChild = current + this->iLeftChild(currentIndex);
        vector<int>::iterator rightChild = current + this->iRightChild(currentIndex);

        this->recursiveMakeHeap(leftChild, begin, end);
        this->recursiveMakeHeap(rightChild, begin, end);

        bool hasChildren = true;
        vector<int>::iterator greatestChild;
        if (leftChild < end && rightChild < end) {
            greatestChild = *leftChild > *rightChild ? leftChild : rightChild;
        } else if (leftChild < end) {
            greatestChild = leftChild;
        } else if (rightChild < end) {
            greatestChild = rightChild;
        } else {
            hasChildren = false;
        }

        if (hasChildren && *greatestChild > *current) {
            int tempCurrent = *current;
            *current = *greatestChild;
            *greatestChild = tempCurrent;
        }

        cout << "At index: " << current - begin << ", data: ";
        vector<int>::iterator it = begin;
        while (it != end) {
            cout << *it << ' ';
            it++;
        }
        cout << endl;

    }
}

void HeapSort::makeHeap(vector<int>::iterator begin, vector<int>::iterator end) {
    if (begin != end) {
        vector<int>::iterator leftChild = begin + this->iLeftChild(0);
        vector<int>::iterator rightChild = begin + this->iRightChild(0);
        cout << "Left child:" << endl;
        this->recursiveMakeHeap(leftChild, begin, end);
        cout << "Right child:" << endl;
        this->recursiveMakeHeap(rightChild, begin, end);
        vector<int>::iterator greatestChild = *leftChild > *rightChild ? leftChild : rightChild;
        if (*greatestChild > *begin) {
            int tempBegin = *begin;
            *begin = *greatestChild;
            *greatestChild = tempBegin;
        }
    }
}

void HeapSort::sort(vector<int> &data) {
    this->makeHeap(data.begin(), data.end());
}