#include <bits/stdc++.h>
using namespace std;

#define PI 2.0*acos(0.0)

class Point{

public:

    double x;
    double y;
    double z;
    double n;

    Point();

    Point(double x, double y, double z);

    Point(double x, double y, double z, double n);

    Point(const Point &p);
    
    Point operator+(const Point p);

    Point operator-(const Point p);

    Point operator*(const double d);

    Point operator/(const double d);

    double operator*(const Point &p);

    Point operator^(const Point &p);

    double length();

    void normalize();

    friend ostream& operator<<(ostream& os, Point& p);

    friend istream& operator>>(istream& is, Point& p);

    friend ofstream& operator<<(ofstream& os, Point& p);
};