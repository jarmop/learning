#include <iostream>
#include <cmath>
#include <vector>

using namespace std;

class Point
{
public:
    // Declare prefix and postfix increment operators.
    Point& operator++();       // Prefix increment operator.
    Point operator++(int);     // Postfix increment operator.

};

// Define prefix increment operator.
Point& Point::operator++()
{
//    _x++;
//    _y++;
    cout << "frere";
    return *this;
}

// Define postfix increment operator.
Point Point::operator++(int)
{
    cout << "ger";
    return *this;
}

int main()
{
    Point* perkele = new Point;

    perkele++;
    ++perkele;

}