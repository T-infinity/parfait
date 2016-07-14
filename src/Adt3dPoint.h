#pragma once
#include "Adt.h"
#include "point.h"
#include "extent.h"
#include <string>
#include <stdexcept>

namespace Parfait {
    class Adt3DPoint {
    public:
        Adt3DPoint(const Extent<double> &domain);
        void store(int Id, const Point<double> &point);
        std::vector<int> retrieve(const Extent<double> &domain) const;

        bool isPointInAdt(const Point<double> &p, int &Id, double tol = 1.0e-8) const {
            auto box = Extent<double>::extentAroundPoint(p, tol);
            auto inside = this->retrieve(box);

            if (0 == inside.size()) {
                return false;
            } else if (1 == inside.size()) {
                Id = inside[0];
                return true;
            } else {
                throw std::domain_error(
                        "Error: Nodes too close trying to create unique mapping of "
                                "There are " + std::to_string(inside.size()) +
                        " nodes inside this box");
            }
        }

        void addPointToAdt(const Point<double> &p, int Id) { this->store(Id, p); }

        int getNodeIdOrAddPoint(const Point<double> &p, int Id) {

            int nodeId = Id;

            if (isPointInAdt(p, nodeId)) {
                return Id;
            } else {
                addPointToAdt(p, nodeId);
                return Id;
            }
        }

        int getNodeId(const Point<double> &p);

    private:
        UnitTransformer unitTransformer;
        Adt<3> adt;
    };
}

#include "Adt3dPoint.hpp"