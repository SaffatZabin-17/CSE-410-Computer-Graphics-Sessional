#include <bits/stdc++.h> 
#include "triangle.h"

using namespace std;

#define PI 2.0*acos(0.0)

Triangle::Triangle(){
    this->a = Point();
    this->b = Point();
    this->c = Point();
}

Triangle::Triangle(Point a, Point b, Point c){
    this->a = a;
    this->b = b;
    this->c = c;
}

void Triangle::sort(){
    if( a.y > b.y ) {
        swap(a,b);
    }
    if( a.y > c.y ) {
        swap(a,c);
    }
    if( b.y > c.y ) {
        swap(b,c);
    }
}

void Triangle::recolor(){
    color.set();
}
