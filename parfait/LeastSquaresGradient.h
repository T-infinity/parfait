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
