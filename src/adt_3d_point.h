#ifndef ADT_3D_POINT_H
#define ADT_3D_POINT_H

#ifndef ADT_H
#include "adt.h"
#endif

#ifndef POINT_H
#include "point.h"
#endif

#ifndef EXTENT_H
#include "extent.h"
#endif

#ifndef UNIT_TRANSFORMER_H
#include "unit_transformer.h"
#endif

#include <string>

#include <stdexcept>

namespace Parfait {
    class Adt3DPoint {
    public:
        Adt3DPoint(const Extent &domain);
        void store(int Id, const Point &point);
        std::vector<int> retrieve(const Extent &domain) const;

        bool isPointInAdt(const Point &p, int &Id, double tol = 1.0e-18) const {
            Extent box = Extent::extentAroundPoint(p, tol);
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

        void addPointToAdt(const Point &p, int Id) { this->store(Id, p); }

        int getNodeIdOrAddPoint(const Point &p, int Id) {

            int nodeId = Id;

            if (isPointInAdt(p, nodeId)) {
                return Id;
            } else {
                addPointToAdt(p, nodeId);
                return Id;
            }
        }

        int getNodeId(const Point &p);

    private:
        UnitTransformer unitTransformer;
        Adt<3> adt;
    };
}

#endif
