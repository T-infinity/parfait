#include "facet.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdexcept>

using namespace Parfait;

double squaredMagnitude(const Point &a){
    return a.pos[0]*a.pos[0] + a.pos[1]*a.pos[1] + a.pos[2]*a.pos[2];
}

void CROSS(double *dest, const double *v1, const double *v2) {
    dest[0] = v1[1]*v2[2]-v1[2]*v2[1];
    dest[1] = v1[2]*v2[0]-v1[0]*v2[2];
    dest[2] = v1[0]*v2[1]-v1[1]*v2[0];
}

double DOT(const double *v1, const double *v2) {
    return (v1[0]*v2[0] + v1[1]*v2[1] + v1[2]*v2[2]);
}

void  SUB(double *dest, const double *v1, const double *v2) {
    dest[0] = v1[0]-v2[0];
    dest[1] = v1[1]-v2[1];
    dest[2] = v1[2]-v2[2];
}

Facet::Facet(const Point &p1, const Point &p2, const Point &p3) {
    points = std::array<Point, 3>{p1, p2, p3};
}

Point &Facet::operator[](int i) {
    if (i < 0 or i > 2)
        throw std::domain_error("Invalid access to facet.");
    return points[i];
}

const Point& Facet::operator[] (int i) const {
    if (i < 0 or i > 2)
        throw std::domain_error("Invalid access to facet.");
    return points[i];
}

double Facet::GetMin(int i){
    double t = points[0][i];
    t = (t < points[1][i])?(t):(points[1][i]);
    t = (t < points[2][i])?(t):(points[2][i]);
    return t;
}

double Facet::GetMax(int i){
    double t = points[0][i];
    t = (t > points[1][i])?(t):(points[1][i]);
    t = (t > points[2][i])?(t):(points[2][i]);
    return t;
}

double Facet::GetXMin(){
    return GetMin(0);
}
double Facet::GetYMin(){
    return GetMin(1);
}
double Facet::GetZMin(){
    return GetMin(2);
}
double Facet::GetXMax(){
    return GetMax(0);
}
double Facet::GetYMax(){
    return GetMax(1);
}
double Facet::GetZMax(){
    return GetMax(2);
}

bool Facet::DoesRayIntersect(const double orig[3],
        const double dir[3], double &t, bool *confident) const{

    double edge1[3], edge2[3], tvec[3], pvec[3], qvec[3];
    double det, inv_det;
    double u, v;

    if(confident != nullptr){
        *confident = true;
    }

    SUB(edge1,&points[1][0],&points[0][0]);
    SUB(edge2,&points[2][0],&points[0][0]);

    /* begin calculating the determinant -also used to calculate U parameter. */
    CROSS(pvec,dir,edge2);

    /* if determinant is near zero, ray lies in plane of triangle. */
    det = DOT(edge1, pvec);

    if(fabs(det) < 1.0e-15){
        return false;
    }

    inv_det = 1.0/det;
    SUB(tvec, orig, &points[0][0]);

    u = DOT(tvec, pvec) *inv_det;
    if(u < 0.0 || u > 1.0){
        return false;
    }

    CROSS(qvec, tvec, edge1);
    v = DOT(dir, qvec) *inv_det;
    if(v < 0.0 || u+v > 1.0){
        return false;
    }

    t = DOT(edge2, qvec)* inv_det;
    if(t < 0.0){
        return false;
    }

    if(confident == nullptr){
        return true;
    }

    //printf("\n u %e, v %e", u, v);
    if(v < 1.0e-12 || v > 1.0+1.0e-12 || u < 1.0e-12 || u > 1.0+1.0e-12){
        *confident = false;
    } else {
        *confident = true;
    }
    return true;
}

bool Facet::WhereDoesEdgeIntersect(const Point &edgelo, const Point &edgehi, Point &point) const {

    double dir[3];
    double dist;
    dir[0] = edgehi[0] - edgelo[0];
    dir[1] = edgehi[1] - edgelo[1];
    dir[2] = edgehi[2] - edgelo[2];
    dist = sqrt(dir[0]*dir[0] + dir[1]*dir[1] + dir[2]*dir[2]);

    double oodist = 1.0/dist;
    dir[0] *= oodist;
    dir[1] *= oodist;
    dir[2] *= oodist;


    double t;
    if(!DoesRayIntersect(&edgelo[0], dir, t)){
        return false;
    }
    if(t > dist){
        return false;
    }

    point[0] = edgelo[0] + dir[0]*t;
    point[1] = edgelo[1] + dir[1]*t;
    point[2] = edgelo[2] + dir[2]*t;
    return true;
}

Extent Facet::getExtent() const{

    Extent extent;
    double x,y,z;
    x = (points[0][0] < points[1][0])?(points[0][0]):(points[1][0]);
    x = (x     < points[2][0])?(x    ):(points[2][0]);
    y = (points[0][1] < points[1][1])?(points[0][1]):(points[1][1]);
    y = (y     < points[2][1])?(y    ):(points[2][1]);
    z = (points[0][2] < points[1][2])?(points[0][2]):(points[1][2]);
    z = (z     < points[2][2])?(z    ):(points[2][2]);

    extent.lo[0] = x;
    extent.lo[1] = y;
    extent.lo[2] = z;

    x = (points[0][0] > points[1][0])?(points[0][0]):(points[1][0]);
    x = (x     > points[2][0])?(x    ):(points[2][0]);
    y = (points[0][1] > points[1][1])?(points[0][1]):(points[1][1]);
    y = (y     > points[2][1])?(y    ):(points[2][1]);
    z = (points[0][2] > points[1][2])?(points[0][2]):(points[1][2]);
    z = (z     > points[2][2])?(z    ):(points[2][2]);

    extent.hi[0] = x;
    extent.hi[1] = y;
    extent.hi[2] = z;

    return extent;
}

void Facet::getNode(int i, double p[3]) const {
    switch (i)  {
        case 0:
            p[0] = points[0][0];
            p[1] = points[0][1];
            p[2] = points[0][2];
            return;
        case 1:
            p[0] = points[1][0];
            p[1] = points[1][1];
            p[2] = points[1][2];
            return;
        case 2:
            p[0] = points[2][0];
            p[1] = points[2][1];
            p[2] = points[2][2];
            return;
    }
}

Point Facet::GetClosestPoint(Point safe, double &dist) const{

    Point point = safe;
    Point diff  =  points[0] - point;
    const Point edge0 =  points[1] - points[0];
    const Point edge1 =  points[2] - points[0];

    double a00 = squaredMagnitude(edge0);
    double a01 = Point::dot(edge0, edge1);
    double a11 = squaredMagnitude(edge1);
    double b0  = Point::dot(diff, edge0);
    double b1  = Point::dot(diff, edge1);
    double det = fabs(a00*a11 - a01*a01);
    double s   = a01*b1 - a11*b0;
    double t   = a01*b0 - a00*b1;
    //double sqrDistance = 1.0e20;
    //  \     |
    //   \reg2|
    //    \   |
    //     \  |
    //      \ |
    //       \|
    //        *points[1]
    //        |\
    //        | \
    //  reg3  |  \ reg1
    //        |   \
    //        |reg0\
    //        |     \
    //        |      \ points[0]
    // -------*-------*------->s
    //        |P0      \
    //  reg4  | reg5    \ reg6

    if (s + t <= det)
    {
        if (s < (double)0)
        {
            if (t < (double)0)  // region 4
            {
                if (b0 < (double)0)
                {
                    t = (double)0;
                    if (-b0 >= a00)
                    {
                        s = (double)1;
                        //sqrDistance = a00 + ((double)2)*b0 + c;
                    }
                    else
                    {
                        s = -b0/a00;
                        //sqrDistance = b0*s + c;
                    }
                }
                else
                {
                    s = (double)0;
                    if (b1 >= (double)0)
                    {
                        t = (double)0;
                        //sqrDistance = c;
                    }
                    else if (-b1 >= a11)
                    {
                        t = (double)1;
                        //sqrDistance = a11 + ((double)2)*b1 + c;
                    }
                    else
                    {
                        t = -b1/a11;
                        //sqrDistance = b1*t + c;
                    }
                }
            }
            else  // region 3
            {
                s = (double)0;
                if (b1 >= (double)0)
                {
                    t = (double)0;
                    //sqrDistance = c;
                }
                else if (-b1 >= a11)
                {
                    t = (double)1;
                    //sqrDistance = a11 + ((double)2)*b1 + c;
                }
                else
                {
                    t = -b1/a11;
                    //sqrDistance = b1*t + c;
                }
            }
        }
        else if (t < (double)0)  // region 5
        {
            t = (double)0;
            if (b0 >= (double)0)
            {
                s = (double)0;
                //sqrDistance = c;
            }
            else if (-b0 >= a00)
            {
                s = (double)1;
                //sqrDistance = a00 + ((double)2)*b0 + c;
            }
            else
            {
                s = -b0/a00;
                //sqrDistance = b0*s + c;
            }
        }
        else  // region 0
        {
            // minimum at interior point
            double invDet = ((double)1)/det;
            s *= invDet;
            t *= invDet;
            //sqrDistance = s*(a00*s + a01*t + ((double)2)*b0) +
            //   t*(a01*s + a11*t + ((double)2)*b1) + c;
        }
    }
    else
    {
        double tmp0, tmpoints, numer, denom;

        if (s < (double)0)  // region 2
        {
            tmp0 = a01 + b0;
            tmpoints = a11 + b1;
            if (tmpoints > tmp0)
            {
                numer = tmpoints - tmp0;
                denom = a00 - ((double)2)*a01 + a11;
                if (numer >= denom)
                {
                    s = (double)1;
                    t = (double)0;
                    //sqrDistance = a00 + ((double)2)*b0 + c;
                }
                else
                {
                    s = numer/denom;
                    t = (double)1 - s;
                    //sqrDistance = s*(a00*s + a01*t + ((double)2)*b0) +
                    //t*(a01*s + a11*t + ((double)2)*b1) + c;
                }
            }
            else
            {
                s = (double)0;
                if (tmpoints <= (double)0)
                {
                    t = (double)1;
                    //sqrDistance = a11 + ((double)2)*b1 + c;
                }
                else if (b1 >= (double)0)
                {
                    t = (double)0;
                    //sqrDistance = c;
                }
                else
                {
                    t = -b1/a11;
                    //sqrDistance = b1*t + c;
                }
            }
        }
        else if (t < (double)0)  // region 6
        {
            tmp0 = a01 + b1;
            tmpoints = a00 + b0;
            if (tmpoints > tmp0)
            {
                numer = tmpoints - tmp0;
                denom = a00 - ((double)2)*a01 + a11;
                if (numer >= denom)
                {
                    t = (double)1;
                    s = (double)0;
                    //sqrDistance = a11 + ((double)2)*b1 + c;
                }
                else
                {
                    t = numer/denom;
                    s = (double)1 - t;
                    //sqrDistance = s*(a00*s + a01*t + ((double)2)*b0) +
                    //t*(a01*s + a11*t + ((double)2)*b1) + c;
                }
            }
            else
            {
                t = (double)0;
                if (tmpoints <= (double)0)
                {
                    s = (double)1;
                    //sqrDistance = a00 + ((double)2)*b0 + c;
                }
                else if (b0 >= (double)0)
                {
                    s = (double)0;
                    //sqrDistance = c;
                }
                else
                {
                    s = -b0/a00;
                    //sqrDistance = b0*s + c;
                }
            }
        }
        else  // region 1
        {
            numer = a11 + b1 - a01 - b0;
            if (numer <= (double)0)
            {
                s = (double)0;
                t = (double)1;
                //sqrDistance = a11 + ((double)2)*b1 + c;
            }
            else
            {
                denom = a00 - ((double)2)*a01 + a11;
                if (numer >= denom)
                {
                    s = (double)1;
                    t = (double)0;
                    //sqrDistance = a00 + ((double)2)*b0 + c;
                }
                else
                {
                    s = numer/denom;
                    t = (double)1 - s;
                    //sqrDistance = s*(a00*s + a01*t + ((double)2)*b0) +
                    //t*(a01*s + a11*t + ((double)2)*b1) + c;
                }
            }
        }
    }

    point = points[0] + s*edge0 + t*edge1;
    dist = Point::distance(safe,point);
    return point;
}

Point Facet::computeNormal() {
    Point a = points[1] - points[0];
    Point b = points[2] - points[0];

    Point c = Point::cross(a, b);
    c.normalize();
    return c;
}

double Facet::getLargestAngleBetween(const std::vector<Facet> &facets) {
    double min = 10e20;
    for(auto fi : facets){
        auto ui = fi.computeNormal();
        for(auto fj : facets){
            auto uj = fj.computeNormal();
            double dot = fabs(Point::dot(ui,uj));
            min = std::min(dot, min);
        }
    }
    return acos(min)*180/3.141592653; //close enough to pi
}
