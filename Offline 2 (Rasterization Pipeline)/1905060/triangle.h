#include <bits/stdc++.h>
#include "point.cpp"
#include "color.h"

using namespace std;

#define PI 2.0*acos(0.0)

class Triangle{
public:

    Point a;
    Point b;
    Point c;
    Color color;

    Triangle();

    Triangle(Point a, Point b, Point c);

    void sort();

    void recolor();
};