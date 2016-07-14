#pragma once
#include <vector>
#include <stdexcept>

namespace Parfait{
    class MeshConnectivity {
    public:
        MeshConnectivity() = default;
        std::vector<int> triangles;
        std::vector<int> quads;
        std::vector<int> tets;
        std::vector<int> pyramids;
        std::vector<int> prisms;
        std::vector<int> hexs;

        int numberOfTriangles() {return triangles.size()/3;}
        int numberOfQuads() {return quads.size()/4;}
        int numberOfTets() {return tets.size()/4;}
        int numberOfPyramids() {return pyramids.size()/5;}
        int numberOfPrisms() {return prisms.size()/6;}
        int numberOfHexes() {return hexs.size() / 8;}

        int* getTrianglePtr(int id) { return &triangles[3*id];}
        int* getQuadPtr(int id) { return &quads[4*id];}
        int* getTetPtr(int id) {return &tets[4*id];};
        int* getPyramidPtr(int id) {return &pyramids[5*id];}
        int* getPrismPtr(int id) {return &prisms[6*id];}
        int* getHexPtr(int id) {return &hexs[8 * id];}

        int numberOfCells() {return numberOfTets()+numberOfPyramids()+numberOfPrisms()+numberOfHexes();}
        int numberOfFaces() {return numberOfTriangles()+numberOfQuads();}
        int sizeOfCell(int id) {
            if(id<numberOfTets()) return 4;
            id -= numberOfTets();
            if(id<numberOfPyramids()) return 5;
            id -= numberOfPyramids();
            if(id<numberOfPrisms()) return 6;
            return 8;
        }
        int sizeOfFace(int id){
            if(id<numberOfTriangles())
                return 3;
            return 4;
        }
        int* getCellPtr(int id) {
            switch(sizeOfCell(id)){
                case 4: return &tets[4*id];
                case 5: return &pyramids[5*convertToInternalId(id)];
                case 6: return &prisms[6*convertToInternalId(id)];
                case 8: return &hexs[8 * convertToInternalId(id)];
            }
            throw std::logic_error("invalid cell size");
        }
        int* getFacePtr(int id){
            if(sizeOfFace(id) == 3)
                return &triangles[3*id];
            return &quads[4*(id-numberOfTriangles())];
        }

    private:
        int convertToInternalId(int id){
            if(id <numberOfTets()) return id;
            id -= numberOfTets();
            if(id <numberOfPyramids()) return id;
            id -= numberOfPyramids();
            if(id <numberOfPrisms()) return id;
            return id - numberOfPrisms();
        }
    };
}