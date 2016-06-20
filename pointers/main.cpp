#include <iostream>

using namespace std;

int main() {
    // regular array access
    string a[3] = {"one", "two", "three"};
    cout << a[1] << endl; // one

    // accessing arrays through pointers
    string *aPtr = a;
    cout << *(aPtr + 1) << endl; // one
    // or just
    cout << *(a + 1) << endl; // one

    // multidimensional arrays with pointers
    string b[2][2] = {
        {"0-0", "0-1"},
        {"1-0", "1-1"}
    };
    cout << b[1][1] << endl; // 1-1

    // b is a pointer to the first row
    // *b is a pointer to the first value of the first row
    // **b is the first value of the first row
    // (b + 1) is a pointer to the second row
    // *(b + 1) is a pointer to the first value of the second row
    // *(*(b + 1) + 1) is a pointer to the second value of the second row
    cout << *(*(b + 1) + 1) << endl; // 1-1
}

