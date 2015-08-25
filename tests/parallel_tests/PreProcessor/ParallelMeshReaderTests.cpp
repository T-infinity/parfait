#include <string>
#include "PreProcessor.h"
#include <ParallelMeshReader.h>

#include "CppUTest/CommandLineTestRunner.h"

using std::string;

TEST_GROUP(ParallelMeshReaderTests) { };

TEST(ParallelMeshReaderTests,Exists) {
    auto mesh = Parfait::ParallelMeshReader::readDistributedGrid({"../../grids/6cell.lb8.ugrid"}, {false});
#if 1
    if(MessagePasser::NumberOfProcesses() == 1){
        LONGS_EQUAL(12, mesh.triangles.size() / 3);
        LONGS_EQUAL(12, mesh.triangleTags.size());
        LONGS_EQUAL(6, mesh.quads.size() / 4);
        LONGS_EQUAL(0, mesh.tets.size() / 4);
        LONGS_EQUAL(0, mesh.pyramids.size() / 5);
        LONGS_EQUAL(6, mesh.prisms.size() / 6);
        LONGS_EQUAL(0, mesh.hexs.size() / 6);
        for(int i = 0; i < 14; i++){
            LONGS_EQUAL(i, mesh.getGlobalNodeId(i));
        }
        for(int i = 0; i < 14; i++){
            LONGS_EQUAL(0, mesh.ownershipDegreeOfLocalNode(i));
        }
        LONGS_EQUAL(14, mesh.numberOfNodesOfDegreeOrUnder(0));
    }
    #endif
}

TEST(ParallelMeshReaderTests, Maps){
    auto reader = Parfait::ParallelMeshReader({"../../grids/6cell.lb8.ugrid"}, {false});
    LONGS_EQUAL(14, reader.totalNumberOfNodes());
    LONGS_EQUAL(1, reader.numberOfGrids());
}

