#include <iostream>

using namespace std;

class Person {
    string name;
    int age;
public:
    void getData() {}
    void putData() {}
};

class Professor : public Person {
    void getData() {

    }
};

class Student : public Person {
    void getData() {

    }
};

int main() {
    cout << "ferfer";
}

