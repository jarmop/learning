#include <iostream>

using namespace std;

void char_test() {
    char c; // 1 byte
    char16_t c16t; // 2 bytes
    char32_t c32t; // 4 bytes
    wchar_t wc32t; // 4 bytes
    signed char signedChar; // 1 byte
    unsigned char unsignedChar; // 1 byte

    cout << "Sizes:" << endl;
    cout << sizeof(c) << endl;
    cout << sizeof(c16t) << endl;
    cout << sizeof(c32t) << endl;
    cout << sizeof(wc32t) << endl;
    cout << sizeof(signedChar) << endl;
    cout << sizeof(unsignedChar) << endl;

    c = 'a';
    c16t = 'a';
    c32t = 'a';
    wc32t = 'a';
    signedChar = 'a';
    unsignedChar = 'a';

    cout << "Initialized with 'a':" << endl;
    cout << c << endl;
    cout << c16t << endl;
    cout << c32t << endl;
    cout << wc32t << endl;
    cout << signedChar << endl;
    cout << unsignedChar << endl;

    c = 97;
    c16t = 97;
    c32t = 97;
    wc32t = 97;
    signedChar = 97;
    unsignedChar = 97;

    cout << "Initialize with 97:" << endl;
    cout << c << endl;
    cout << c16t << endl;
    cout << c32t << endl;
    cout << wc32t << endl;
    cout << signedChar << endl;
    cout << unsignedChar << endl;
}

void integer_test() {
    short s;
    int i;
    long l;
    long long l2;
    unsigned short us;
    unsigned int ui;
    unsigned long ul;
    unsigned long long ul2;

    cout << "Sizes (signed):" << endl;
    cout << sizeof(s) << endl;
    cout << sizeof(i) << endl;
    cout << sizeof(l) << endl;
    cout << sizeof(l2) << endl;
    cout << sizeof(us) << endl;
    cout << sizeof(ui) << endl;
    cout << sizeof(ul) << endl;
    cout << sizeof(ul2) << endl;

    s = 1;
    i = 1;
    l = 1;
    l2 = 1;
    us = 1;
    ui = 1;
    ul = 1;
    ul2 = 1;

    cout << "Initialized with 1:" << endl;
    cout << s << endl;
    cout << i << endl;
    cout << l << endl;
    cout << l2 << endl;
    cout << us << endl;
    cout << ui << endl;
    cout << ul << endl;
    cout << ul2 << endl;

    cout << "Uninitialized integer is the same as an integer initialized as zero" << endl;
    int not_initialized;
    int initialied_as_zero = 0;
    cout << not_initialized << endl;
    cout << initialied_as_zero << endl;
}

int main() {
//    char_test();
//    integer_test();
    cout << "Left \a Right" << endl;

};