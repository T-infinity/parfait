#include <string>
#include "PreProcessor.h"
#include <ParallelMeshReader.h>

#include "CppUTest/CommandLineTestRunner.h"

using std::string;

TEST_GROUP(ParallelMeshReaderTests) { };

TEST(ParallelMeshReaderTests,Exists) {
    auto mesh = Parfait::ParallelMeshReader::readDistributedGrid({"../../../run/6cell.lb8.ugrid"}, {false});
}

TEST(ParallelMeshReaderTests, Maps){
    auto reader = Parfait::ParallelMeshReader({"../../../run/6cell.lb8.ugrid"}, {false});
    LONGS_EQUAL(14, reader.totalNumberOfNodes());
    LONGS_EQUAL(1, reader.numberOfGrids());
}

