#include "MeshParallelMetadata.h"
#include "catch.hpp"


TEST_CASE("Get boundary tags by face number"){
    Parfait::MeshParallelMetaData metaData;
    metaData.quadTags = {9,8};
    metaData.triangleTags = {4,5};

    REQUIRE(5 == metaData.getTagForFace(1));
}

