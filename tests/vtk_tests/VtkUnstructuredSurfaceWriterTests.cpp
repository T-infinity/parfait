#include <catch.hpp>
#include <vector>
#include <Point.h>
#include "VtkUnstructuredSurfaceWriter.h"

using namespace std;

class MockVtkSurfaceMesh{
public:
    int numberOfNodes() {return 7;}
    Parfait::Point<double> getNode(int i){
        switch(i){
            case 0: return {0,0,0};
            case 1: return {1,0,0};
            case 2: return {1,1,0};
            case 3: return {0,0,1};
            case 4: return {1,0,1};
            case 5: return {1,1,1};
            case 6: return {1,1,1};
        }
        throw std::logic_error("bad choice");
    }
    int numberOfBoundaryFaces(){return 2;}
    vector<int> getNodesInBoundaryFace(int i) {return i==0?vector<int>({0,1,2}):vector<int>({3,4,5,6});}
private:
};

TEST_CASE("can create a vizualization for a surface mesh"){
    MockVtkSurfaceMesh mesh;
    Parfait::VtkUnstructuredSurfaceWriter writer("surface",mesh);
    writer.writeBinary();
}

