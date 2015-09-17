#include "VtkUnstructuredWriter.h"
#include "CartBlock.h"
#include "catch.hpp"

using namespace Parfait;

class A_Tet{
public:
    A_Tet(){};
    int numberOfNodes(){return 4;}
    int numberOfCells(){return 1;}
    Parfait::Point<double> getNode(int i){
        if(0 == i)
            return {0,0,0};
        if(1 == i)
            return {1,0,0};
        if(2 == i)
            return {0,1,0};
        if(3 == i)
            return {0,0,1};
    }
    std::vector<int> getNodesInCell(int i){return {0,1,2,3};}
}a_tet;

TEST_CASE("create viz file"){
    CartBlock block({0,0,0},{1,1,1},5,5,5);
    VtkUnstructuredWriter writer("test",block);
    writer.writeBinary();
}