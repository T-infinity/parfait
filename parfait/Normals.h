#pragma once
#include <vector>
#include "Point.h"

namespace Parfait {

  namespace Normals {
    inline Parfait::Point<double> average(const std::vector<Parfait::Point<double>> &normals) {
        Parfait::Point<double> norm = {0, 0, 0};

        for (auto n : normals) {
            n.normalize();
            norm += n;
        }
        norm.normalize();

        return norm;
    }

    inline double angleBetweenInRad(const Parfait::Point<double> &u, const Parfait::Point<double> &v) {
        auto d = Parfait::Point<double>::dot(u, v);
        d /= (u.magnitude() * v.magnitude());
        return std::acos(d);
    }

    inline double averageAngleInRad(const Parfait::Point<double> &norm, const std::vector<Parfait::Point<double>> &normals) {
        double avg = 0;
        for (auto u : normals) {
            avg += angleBetweenInRad(u, norm);
        }
        return avg / normals.size();
    }

    inline Parfait::Point<double> goodNormal(const std::vector<Parfait::Point<double>> &normals) {
        auto norm = average(normals);

        for (int i = 0; i < 100000; i++) {
            auto average_angle = averageAngleInRad(norm, normals);
            double max_delta = 0;
            auto updated_normal = norm;
            for (auto u : normals) {
                auto angle = angleBetweenInRad(u, norm);
                auto delta = average_angle - angle;
                updated_normal += u * fabs(delta);
                max_delta = std::max(delta, max_delta);
            }
            norm = updated_normal;
            norm.normalize();
            if (max_delta < 1.0e-8)
                break;
        }

        return norm;
    }
  }
}


