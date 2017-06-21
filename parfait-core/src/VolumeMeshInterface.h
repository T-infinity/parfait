#pragma once

#include <vector>
#include "Point.h"

namespace Parfait {
    class VolumeMeshInterface {
        public:
            virtual int numberOfNodes() const = 0;
            virtual int numberOfCells() const = 0;
            virtual std::vector<int> getNodesInCell(int cellId) const = 0;
            virtual Point<double> getPoint(int nodeId) const = 0;
    };
}

