#include <iostream>
#include <cmath>

using namespace std;

int main() {
    long long n = 10000000;
    long long f[n];

    long long rt = pow(2,31);

    for (long long i=0; i<n; i++) {
        if (i%100000==0) {
            cout << i << endl;
        }
    }

    return 0;

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

    // multidimensional arrays with variable sized rows
    string* c[2];
    string c1[2] = {"0-0", "0-1"};
    string c2[5] = {"1-0", "1-1", "1-2", "1-3", "1-4"};
    c[0] = c1;
    c[1] = c2;
    cout << c[1][1] << endl;

    // initializing multidimensional array with variable sized rows in a for loop
    string* d[2];

    cout << sizeof(c)/ sizeof(*c) << endl;
    cout << sizeof(c[0])/ sizeof(*c[0]) << endl;
    cout << sizeof(c) << endl;
    cout << sizeof(*c) << endl;
    cout << sizeof(c[0]) << endl;
    cout << sizeof(*c[0]) << endl;

//    string p = "perkele";
//    string p = "p";
//    char p = "p";
//    cout << sizeof(p) << endl;

    for (int i=0; i<sizeof(c)/sizeof(*c); i++) {
        for (int j=0; j<sizeof(c[i])/sizeof(*c[i]); i++) {

        }
    }
}

