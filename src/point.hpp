#include <stdio.h>
#include <assert.h>
#include <math.h>

inline Point::Point(){
}

inline Point::Point(double a, double b, double c){
    pos[0] = a;
    pos[1] = b;
    pos[2] = c;
}


inline Point::Point(const double *p){
    pos[0] = p[0];
    pos[1] = p[1];
    pos[2] = p[2];
}

inline void Point::Print() const 
{
    printf(" %e %e %e", pos[0], pos[1], pos[2]);
}

inline Point Point::operator+(const Point &rhs) const 
{
    return Point(
            pos[0] + rhs.pos[0], 
            pos[1] + rhs.pos[1], 
            pos[2] + rhs.pos[2]);
}

inline Point Point::operator-(const Point &rhs) const 
{
    return Point(
            pos[0] - rhs.pos[0], 
            pos[1] - rhs.pos[1], 
            pos[2] - rhs.pos[2]);
}

inline Point Point::operator*(const double a) const 
{
    return Point(
            a*pos[0], 
            a*pos[1], 
            a*pos[2]);
}

inline Point Point::operator/(const double a) const 
{
    double ooa = 1.0/a;
    return Point(
            ooa*pos[0], 
            ooa*pos[1], 
            ooa*pos[2]);
}

inline Point Point::operator*=(const double a)
{
    pos[0] *= a;
    pos[1] *= a;
    pos[2] *= a;
    return *this;
}

inline Point Point::operator/=(const double a)
{
    double b = 1.0/a;
    return *this *= b;
}

inline Point operator*(double a, const Point &rhs){
    return Point(
            a*rhs.pos[0],
            a*rhs.pos[1],
            a*rhs.pos[2]);
}

inline Point Point::operator+=(const Point &rhs){
    pos[0] += rhs.pos[0];
    pos[1] += rhs.pos[1];
    pos[2] += rhs.pos[2];
    return *this;
}

inline Point Point::operator-=(const Point &rhs){
    pos[0] -= rhs.pos[0];
    pos[1] -= rhs.pos[1];
    pos[2] -= rhs.pos[2];
    return *this;
}

inline bool Point::operator==(const Point &rhs){
    if(pos[0] != rhs.pos[0])
        return false;
    if(pos[1] != rhs.pos[1])
        return false;
    if(pos[2] != rhs.pos[2])
        return false;
    return true;
}

inline bool Point::operator!=(const Point &rhs){
    if(pos[0] != rhs.pos[0])
        return true;
    if(pos[1] != rhs.pos[1]) 
        return true;
    if(pos[2] != rhs.pos[2])
        return true;
    return false;
}

inline bool Point::approxEqual(const Point &rhs, double tol) const{
    if(pos[0] +tol < rhs.pos[0] or pos[0] - tol > rhs.pos[0])
        return false;
    if(pos[1] +tol < rhs.pos[1] or pos[1] - tol > rhs.pos[1])
        return false;
    if(pos[2] +tol < rhs.pos[2] or pos[2] - tol > rhs.pos[2])
        return false;
    return true;
}

inline double Point::distance(const Point &a, const Point &b){
    return sqrt(
            (b.pos[0]-a.pos[0])*(b.pos[0]-a.pos[0]) + 
            (b.pos[1]-a.pos[1])*(b.pos[1]-a.pos[1]) + 
            (b.pos[2]-a.pos[2])*(b.pos[2]-a.pos[2]));
}

inline double Point::dot(const Point &a, const Point &b){
    return a.pos[0]*b.pos[0] + a.pos[1]*b.pos[1] + a.pos[2]*b.pos[2];
}


inline double Point::magnitude(const Point &a){
    return sqrt(a[0]*a[0] + a[1]*a[1] + a[2]*a[2]);
}

inline Point Point::cross(const Point &a, const Point &b){
    return Point(
            a[1]*b[2] - b[1]*a[2],
            b[0]*a[2] - a[0]*b[2],
            a[0]*b[1] - b[0]*a[1]
            );
}

inline void Point::normalize() {
    double mag = magnitude(*this);
    *this /= mag;
}

inline Point Point::normalize(const Point &a){
    Point b{a};
    b.normalize();
    return b;
}
