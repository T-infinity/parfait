#ifndef PARFAIT_MESHDATA_H
#define PARFAIT_MESHDATA_H
#include <vector>
namespace Parfait{
    class MeshConnectivity {
    public:
        MeshConnectivity() = default;
        std::vector<int> triangles;
        std::vector<int> quads;
        std::vector<int> tets;
        std::vector<int> pyramids;
        std::vector<int> prisms;
        std::vector<int> hexes;
    };
}
#endif 
