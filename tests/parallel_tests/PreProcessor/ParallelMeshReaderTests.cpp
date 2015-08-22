#include <string>
#include "PreProcessor.h"
#include <ParallelMeshReader.h>

#include "CppUTest/CommandLineTestRunner.h"

using std::string;

TEST_GROUP(ParallelMeshReaderTests)
{ };

TEST(ParallelMeshReaderTests,Exists)
{
    auto mesh = Parfait::ParallelMeshReader::readDistributedGrid({"../../../run/6cell.lb8.ugrid"}, {false});
}

