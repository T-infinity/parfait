
// Copyright 2016 United States Government as represented by the Administrator of the National Aeronautics and Space Administration. 
// No copyright is claimed in the United States under Title 17, U.S. Code. All Other Rights Reserved.
// 
// The “Parfait: A Toolbox for CFD Software Development [LAR-18839-1]” platform is licensed under the 
// Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. 
// You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0. 
// 
// Unless required by applicable law or agreed to in writing, software distributed under the License is distributed 
// on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. 
// See the License for the specific language governing permissions and limitations under the License.
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

        std::string toString() const;
    private:
        double mat[16];
    };
}

#include "MotionMatrix.hpp"
