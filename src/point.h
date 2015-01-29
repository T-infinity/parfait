#ifndef POINT_H
#define POINT_H

class Point
{
    public:
        double pos[3];

        Point();
        Point(double x, double y, double z);
		template<typename T>
        Point(const T& p);
        double *data() { return pos; }
        const double *data() const { return pos; }

        inline double &operator[] (int i) 
        {
            return pos[i];
        }
        inline const double &operator[] (int i) const 
        {
            return pos[i];
        }
        Point operator+(const Point &rhs) const;
        Point operator-(const Point &rhs) const;
        Point operator*(const double a) const;
        Point operator/(const double a) const;
        Point operator*=(const double a);
        Point operator/=(const double a);
        Point operator+=(const Point &rhs);
        Point operator-=(const Point &rhs) ;
        bool operator==(const Point &rhs);
        bool operator!=(const Point &rhs);
        bool approxEqual(const Point &rhs, double tol = 1.0e-14) const;
        void normalize();
        friend Point operator*(double a, const Point &rhs);

        static double distance(const Point &a, const Point &b);
        static double dot(const Point &a, const Point &b);
        static double magnitude(const Point &a);
        static Point cross(const Point &a, const Point &b);
        static Point normalize(const Point &a);

        void Print() const;
};

#include "point.hpp"

#endif
