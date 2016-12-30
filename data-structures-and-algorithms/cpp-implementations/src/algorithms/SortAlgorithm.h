//
// Created by Jarmo Pihlajaniemi on 26/06/16.
//

#ifndef DATA_STRUCTURES_AND_ALGORITHMS_SORTALGORITHM_H
#define DATA_STRUCTURES_AND_ALGORITHMS_SORTALGORITHM_H

#include <vector>
#include <list>

using namespace std;

class SortAlgorithm {
public:
    virtual void sort(vector<int> &data) {};
};

/**
 * 10k: 1.2 s, 20k: ~4.38 s, 30k: ~9 s, 40k: ~17 s => O(n^2)
 */
class BubbleSort : public SortAlgorithm {
public:
    void sort(vector<int> &data);

};

/**
 * 10k: 1.6 s, 20k: ~4.45 s, 30k: ~10 s, 40k: 25 s => O(n^2)
 * Lost to bubble sort :(
 */
class CustomSort : public SortAlgorithm {
public:
    void sort(vector<int> &data);
};

/**
 * 10k: 0.12 s, 20k: 0.30 s, 30k: 0.47 s, 40k: 0.58 s => O(n log n)
 * Wins bubble sort easily if n is over few thousands
 */
class MergeSort : public SortAlgorithm {
    vector<list<int>> merge(vector<list<int>> &container);
    list<int> mergeParts(list<int> part1, list<int> part2);
public:
    void sort(vector<int> &data);
};


#endif //DATA_STRUCTURES_AND_ALGORITHMS_SORTALGORITHM_H
