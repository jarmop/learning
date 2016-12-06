#include <iostream>
using namespace std;

/**
 * Example Google interview questions mentioned at Glassdoor:
 * https://www.glassdoor.com/Interview/Google-Software-Engineer-Interview-Questions-EI_IE9079.0,6_KO7,24.htm#InterviewReview_12218814
 */

/**
 * Phone interview:
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

/**
 * Onsite 1:
 * Given a node in a BST where each node has a pointer to its parent, find the next largest node.
 */

/**
 * Onsite 2:
 * Implement String encode and decode
 */

/**
 * Onsite 3:
 * Given an int array A, return an int array B that A[i + B[i]] is the first element in A after A[i] that is greater than or equal to A[i].
 */
void onsite3() {
    int A[] = {5,3,1,45,5,6,7,45};
    int sizeA = 8;
    int B[sizeA];
    for (int i = 0; i < sizeA; i++) {
        for (int j = i + 1; j < sizeA; j++) {
            if (A[j] >= A[i]) {
                B[i] = j;
            }
        }
    }

    cout << B[4] << endl;
}


/**
 * Onsite 4:
 * Given a list of files, each of which contains some "require METHOD_NAME" lines, and "provide METHOD_NAME" lines. Return a working order of files to be processed.
 */

/**
 * Onsite 5:
 * Implement a telephone # management system that assign randomly, recycle and check availability of 10 digit numbers.
 */

int main() {
//    findPairWithSum();
    onsite3();
}