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

        int numberOfTriangles() {return triangles.size()/3;}
        int numberOfQuads() {return quads.size()/4;}
        int numberOfTets() {return tets.size()/4;}
        int numberOfPyramids() {return pyramids.size()/5;}
        int numberOfPrisms() {return prisms.size()/6;}
        int numberOfHexes() {return hexes.size()/8;}

        int* getTrianglePtr(int id) { return &triangles[3*id];}
        int* getQuadPtr(int id) { return &quads[4*id];}
        int* getTetPtr(int id) {return &tets[4*id];};
        int* getPyramidPtr(int id) {return &pyramids[5*id];}
        int* getPrismPtr(int id) {return &prisms[6*id];}
        int* getHexPtr(int id) {return &hexes[8*id];}
    };
}
#endif 
