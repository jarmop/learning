#include <iostream>
#include <set>
#include <vector>

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

// First go through each file and find unique methods that are required somewhere
// Find files where required methods are provided
// Map each file to the files from where they require a method
// A B C D E

/**
 * Onsite 5:
 * Implement a telephone # management system that assign randomly, recycle and check availability of 10 digit numbers.
 */

/* Another interview set
 * https://www.glassdoor.com/Interview/Google-Software-Engineer-Interview-Questions-EI_IE9079.0,6_KO7,24.htm#InterviewReview_12617243
 * Given an NxN matrix how would you print all elements in a spiral way starting from the middle element.
 */
void printOuterRing(int matrix[][4], int start, int end) {
    cout << "print left side" << endl;
    int j = start;
    for (int i = start; i <= end; i++) {
        cout << matrix[i][j] << endl;
    }
    cout << "print bottom side" << endl;
    int i = end;
    for (int j = start + 1; j <= end; j++) {
        cout << matrix[i][j] << endl;
    }
    cout << "print right side" << endl;
    j = end;
    for (int i = end - 1; i >= start; i--) {
        cout << matrix[i][j] << endl;
    }
    cout << "print top side" << endl;
    i = start;
    for (int j = end - 1; j >= start + 1; j--) {
        cout << matrix[i][j] << endl;
    }
}

void phone_screen() {
    const int n = 4;
    int matrix[4][4] = {
            {1,2,3,4},
            {5,6,7,8},
            {9,10,11,12},
            {13,14,15,16}
    };
//    int matrix[3][3] = {
//            {1,2,3},
//            {4, 5,6},
//            {7,8,9}
//    };

    int start;
    int end;
    if (n % 2 == 0) {
        start = n / 2 - 1;
        end = start + 1;
    } else {
        start = n / 2 - 1;
        end = start + 1;
    }

    for (start; start >= 0; start--) {
        printOuterRing(matrix, start, end);
        end++;
    }
}


// Q1: Find all pairs of numbers that are equal to a given sum in a given int array
// n log n
void onsite2_int1_q1() {
    vector<int> vec {1,5,3,7,11,5,6,-3,-1};
    int sum = 8;
    set<int> found_nums;
    for (int i = 0; i < vec.size(); i++) {
        int complement = sum - vec[i];
        if (found_nums.find(complement) != found_nums.end()) {
            cout << vec[i] << "+" << complement << endl;
        } else {
            found_nums.insert(vec[i]);
        }
    }
}

int main() {
//    findPairWithSum();
//    onsite3();
    phone_screen();
//    onsite2_int1_q1();
}