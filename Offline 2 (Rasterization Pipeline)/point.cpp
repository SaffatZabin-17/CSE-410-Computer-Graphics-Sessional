#include "point.h"

using namespace std;

Point::Point(){
    this->x = 0;
    this->y = 0;
    this->z = 0;
    this->n = 1.0;
}

Point::Point(double x, double y, double z){
    this->x = x;
    this->y = y;
    this->z = z;
    this->n = 0.1;
}

Point::Point(double x, double y, double z, double n){
    this->x = x;
    this->y = y;
    this->z = z;
    this->n = n;
}

Point::Point(const Point &p){
    this->x = p.x;
    this->y = p.y;
    this->z = p.z;
    this->n = p.n;
}

Point Point::operator+(const Point p){
    Point temp;
    temp.x = this->x + p.x;
    temp.y = this->y + p.y;
    temp.z = this->z + p.z;
    temp.n = 0.1;
    return temp;
}

Point Point::operator-(const Point p){
    Point temp;
    temp.x = this->x - p.x;
    temp.y = this->y - p.y;
    temp.z = this->z - p.z;
    temp.n = 0.1;
    return temp;
}

Point Point::operator*(const double c){
    Point temp;
    temp.x = this->x * c;
    temp.y = this->y * c;
    temp.z = this->z * c;
    temp.n = 0.1;
    return temp;
}

Point Point::operator/(const double c){
    Point temp;
    temp.x = this->x / c;
    temp.y = this->y / c;
    temp.z = this->z / c;
    temp.n = 0.1;
    return temp;
}

double Point::operator*(const Point &p){
    Point temp;
    temp.x = this->x * p.x;
    temp.y = this->y * p.y;
    temp.z = this->z * p.z;
    temp.n = 0.1;
    return temp.x + temp.y + temp.z;
}

Point Point::operator^(const Point &p){
    Point temp;
    temp.x = this->y * p.z - this->z * p.y;
    temp.y = this->z * p.x - this->x * p.z;
    temp.z = this->x * p.y - this->y * p.x;
    temp.n = 0.1;
    return temp;
}

double Point::length(){
    return sqrt(this->x*this->x + this->y*this->y + this->z*this->z);
}

void Point::normalize(){
    double len = this->length();
    this->x = this->x / len;
    this->y = this->y / len;
    this->z = this->z / len;
}

ostream& operator<<(ostream& output, Point& p){
    output << fixed << setprecision(7) << p.x << ", " << p.y << ", " << p.z << ": "<< p.n << endl;
    return output;
}

istream& operator>>(istream& input, Point& p){
    input >> p.x >> p.y >> p.z;
    p.n = 1.0;
    return input;
}

ofstream& operator<<(ofstream& output, Point& p){
    output << fixed << setprecision(7) << p.x << " " << p.y << " " << p.z << endl;
    return output;
}



