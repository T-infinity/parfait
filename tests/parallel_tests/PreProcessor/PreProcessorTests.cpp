#include <string>
#include <ParallelMeshReader.h>
#include "PreProcessor.h"
#include <catch.hpp>

using std::string;
using namespace Parfait;

TEST_CASE("Read in grid based on xml file (on one proc)") {
    ConfigurationReader reader("6cell.xml");
    Configuration config = reader.createConfiguration();

    PreProcessor preProcessor(config);
    auto mesh = preProcessor.createMesh();

    if(MessagePasser::NumberOfProcesses() == 1){
        REQUIRE(12 == (mesh->connectivity->triangles.size() / 3));
        REQUIRE(12 == mesh->metaData->triangleTags.size());
        REQUIRE(6 == (mesh->connectivity->quads.size() / 4));
        REQUIRE(0 == (mesh->connectivity->tets.size() / 4));
        REQUIRE(0 == (mesh->connectivity->pyramids.size() / 5));
        REQUIRE(6 == (mesh->connectivity->prisms.size() / 6));
        REQUIRE(0 == (mesh->connectivity->hexes.size() / 6));
        for(int i = 0; i < 14; i++){
            REQUIRE(i == mesh->metaData->globalNodeIds[i]);
        }
        for(int i = 0; i < 14; i++){
            REQUIRE(0 == mesh->metaData->nodeOwnershipDegree[i]);
        }
    }
}
