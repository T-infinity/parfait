
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
#include <vector>

namespace Parfait {

    struct GSWeights {
    public:
        inline GSWeights(double a, double b, double c, double d, double e, double f)
                : r11(a), r12(b), r13(c), r22(d), r23(e), r33(f) {}

        double r11, r12, r13, r22, r23, r33;
    };

    template<typename PointCollection, typename Point>
    auto calcGramSchmidtWeights(const PointCollection &points, const Point &center);

    template<typename PointCollection, typename Point>
    auto calcLeastSquaresGradient(const PointCollection &points, const Point &center, const GSWeights &gs,
                                  const std::vector<double> &u, double u0);

}
#include "LeastSquaresGradient.hpp"
