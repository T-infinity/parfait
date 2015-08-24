#ifndef PARFAIT_CELLMESSAGER_H
#define PARFAIT_CELLMESSAGER_H

#include <vector>
class CellMessager {
public:
    CellMessager(std::vector<int> &triangles, std::vector<int> &triangleTags);
    void sendTriangles(const std::vector<int> &packedTriangles);
private:
    std::vector<int> &triangles;
};

#include "CellMessager.hpp"

#endif
