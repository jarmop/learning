/**
Write a function that takes a string as input and returns the string reversed.

Example:
Given s = "hello", return "olleh".

https://leetcode.com/problems/reverse-string/
*/

#include <iostream>
using namespace std;

void reverseString(string::iterator begin, string::iterator end) {
    end--;
    while (begin < end) {
        char temp = *begin;
        *begin = *end;
        *end = temp;
        cout << temp << endl;
        begin++;
        end--;
    }
}

int main() {
    string s = "test";
    reverseString(s.begin(), s.end());

    cout << s << endl;
}