#pragma once
#include <assert.h>
#include "ContiguousMatrixMultiplication.h"
#include <math.h>
#include <stdio.h>

namespace Parfait {
    class MotionMatrix {
    public:
        MotionMatrix();
        MotionMatrix(const double translation[3]);
        MotionMatrix(double dx, double dy, double dz);
        MotionMatrix(const double line_start[3], const double line_end[3], double angle);

        void clearMotions();
        void setTranslation(const double translation[3]);
        void addTranslation(const double translation[3]);
        void setRotation(const double line_start[3], const double line_end[3], double angle);
        void addRotation(const double line_start[3], const double line_end[3], double angle);
        void addMotion(const MotionMatrix &motion);
        void setMotionMatrix(const double matrix[16]);
        void getMatrix(double matrix[16]) const;
        void movePoint(double p[3]) const;

        void printMatrix(FILE *f) const;
    private:
        double mat[16];
    };
}

#include "MotionMatrix.hpp"