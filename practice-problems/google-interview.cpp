#include <iostream>
using namespace std;

/**
 * Given two int arrays A and B, and an int c, return the total number of pairs (a, b) where a is from A and b is from B, and a + b <=c
 */

void findPairWithSum() {
    int A[] = {1,23,3,7};
    int B[] = {5,0,33,7};
    int c = 13;

    int sizeA = sizeof(A)/sizeof(*A);
    int sizeB = sizeof(B)/sizeof(*B);

    int count = 0;
    for (int i = 0; i < sizeA; i++) {
        for (int j = 0; j < sizeB; j++) {
            if (A[i] + B[j] <= c) {
                count++;
                cout << A[i] << " + " << B[j] << endl;
            }
        }
    }

    cout << count << endl;
}

int main() {
    findPairWithSum();
}