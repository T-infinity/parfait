#include "point.h"
#include <CppUTest/CommandLineTestRunner.h>

TEST_GROUP(Point){

};

TEST(Point, Constructor){

	// Point no longer initializes to zero for performance. 
	// It is just as "safe" now as double [3]
    //Point a;
    //CHECK_EQUAL(a[0], 0);
    //CHECK_EQUAL(a[1], 0);
    //CHECK_EQUAL(a[2], 0);

    Point b(1.0, 2.0, 3.0);
    CHECK_EQUAL(1.0, b[0]);
    CHECK_EQUAL(2.0, b[1]);
    CHECK_EQUAL(3.0, b[2]);
}

TEST(Point, DoubleConstructor)
{
    double p[3] = {1,2,3};
    Point a(p);

    CHECK_EQUAL(1.0, a[0]);
    CHECK_EQUAL(2.0, a[1]);
    CHECK_EQUAL(3.0, a[2]);
}

TEST(Point, listConstructor)
{
    Point a({1,2,3});

    CHECK_EQUAL(1.0, a[0]);
    CHECK_EQUAL(2.0, a[1]);
    CHECK_EQUAL(3.0, a[2]);
}

TEST(Point, addition){

    Point a(1,2,3);
    Point b = a+a;
    CHECK_EQUAL(2, b[0]);
    CHECK_EQUAL(4, b[1]);
    CHECK_EQUAL(6, b[2]);
}

TEST(Point, subtraction){
    Point a(1,2,3);
    Point b(3,2,1);
    Point c = a-b;

    CHECK_EQUAL(-2, c[0]);
    CHECK_EQUAL( 0, c[1]);
    CHECK_EQUAL( 2, c[2]);
}

TEST(Point, scaling){

    Point a(1,2,3);
    Point b = a*0.5;

    CHECK_EQUAL(0.5, b[0]);
    CHECK_EQUAL(1.0, b[1]);
    CHECK_EQUAL(1.5, b[2]);

    b = 0.5*a;
    CHECK_EQUAL(0.5, b[0]);
    CHECK_EQUAL(1.0, b[1]);
    CHECK_EQUAL(1.5, b[2]);
}

TEST(Point, distance){
    Point a(1,0,0);
    Point b(0,0,0);

    double distance = Point::distance(a, b);
    CHECK_EQUAL(1, distance);
}

TEST(Point, dot){
    Point a(1,1,1);
    double dot = Point::dot(a,a);
    CHECK_EQUAL(3, dot);
}

TEST(Point, scalingInPlace){
    Point a(1,2,3);
    a *= 0.5;
    CHECK_EQUAL(0.5, a[0]);
    CHECK_EQUAL(1.0, a[1]);
    CHECK_EQUAL(1.5, a[2]);
}

TEST(Point, cross){

    Point a(1,0,0);
    Point b(0,1,0);
    Point c(0,0,1);
    Point z = Point::cross(a, b);
    double tol = 1.0e-14;
    DOUBLES_EQUAL(c[0], z[0], tol);
    DOUBLES_EQUAL(c[1], z[1], tol);
    DOUBLES_EQUAL(c[2], z[2], tol);
}

