#include "VtkUnstructuredWriter.h"
#include "CartBlock.h"
#include "catch.hpp"

using namespace Parfait;

TEST_CASE("create viz file"){
    CartBlock block({0,0,0},{1,1,1},5,5,5);
    VtkUnstructuredWriter writer("test",block);
    writer.writeBinary();
}