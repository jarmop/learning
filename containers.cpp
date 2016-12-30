#include <iostream>
#include <vector>

using namespace std;

int main() {
//    vector<int> data;
//
//    data.push_back(4);
////    data.push_back(5);
//
//    auto begin = data.begin();
//    auto end = data.end();
//    auto it1 = begin + 1;
//    auto it2 = begin + 2;
//
//    cout << (begin == end) << endl;
//    cout << (it1 >= end) << endl;
//    cout << (it2 >= end) << endl;
//
//    cout << *begin;

    vector<int> data;
    data.push_back(1);

    auto& elem = data[0];

      // seg fault if value is not inserted before assigning elem
//    data.push_back(1);

    cout << elem;
}