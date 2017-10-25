#include <catch.hpp>
#include "CGNSFaceExtraction.h"

TEST_CASE("Get tet faces"){
    std::vector<int> cell {99,88,77,66};
    std::vector<int> face;
    Parfait::CGNS::getTetFace(cell,0,face);

    REQUIRE(3 == face.size());
    REQUIRE(99 == face[0]);
    REQUIRE(77 == face[1]);
    REQUIRE(88 == face[2]);

    Parfait::CGNS::getTetFace(cell,1,face);
    REQUIRE(3 == face.size());
    REQUIRE(99 == face[0]);
    REQUIRE(88 == face[1]);
    REQUIRE(66 == face[2]);

    Parfait::CGNS::getTetFace(cell,2,face);
    REQUIRE(3 == face.size());
    REQUIRE(88 == face[0]);
    REQUIRE(77 == face[1]);
    REQUIRE(66 == face[2]);

    Parfait::CGNS::getTetFace(cell,3,face);
    REQUIRE(3 == face.size());
    REQUIRE(77 == face[0]);
    REQUIRE(99 == face[1]);
    REQUIRE(66 == face[2]);
}

TEST_CASE("Get pyramid faces") {
    std::vector<int> cell{99, 88, 77, 66, 55};
    std::vector<int> face;

    Parfait::CGNS::getPyramidFace(cell, 0, face);
    REQUIRE(4 == face.size());
    REQUIRE(99 == face[0]);
    REQUIRE(66 == face[1]);
    REQUIRE(77 == face[2]);
    REQUIRE(88 == face[3]);

    Parfait::CGNS::getPyramidFace(cell, 1, face);
    REQUIRE(3 == face.size());
    REQUIRE(99 == face[0]);
    REQUIRE(88 == face[1]);
    REQUIRE(55 == face[2]);

    Parfait::CGNS::getPyramidFace(cell, 2, face);
    REQUIRE(3 == face.size());
    REQUIRE(88 == face[0]);
    REQUIRE(77 == face[1]);
    REQUIRE(55 == face[2]);

    Parfait::CGNS::getPyramidFace(cell, 3, face);
    REQUIRE(3 == face.size());
    REQUIRE(77 == face[0]);
    REQUIRE(66 == face[1]);
    REQUIRE(55 == face[2]);

    Parfait::CGNS::getPyramidFace(cell, 4, face);
    REQUIRE(3 == face.size());
    REQUIRE(66 == face[0]);
    REQUIRE(99 == face[1]);
    REQUIRE(55 == face[2]);
}

TEST_CASE("Get prism faces") {
    std::vector<int> cell{99, 88, 77, 66, 55, 44};
    std::vector<int> face;

    Parfait::CGNS::getPrismFace(cell, 0, face);
    REQUIRE(4 == face.size());
    REQUIRE(99 == face[0]);
    REQUIRE(88 == face[1]);
    REQUIRE(55 == face[2]);
    REQUIRE(66 == face[3]);

    Parfait::CGNS::getPrismFace(cell, 1, face);
    REQUIRE(4 == face.size());
    REQUIRE(88 == face[0]);
    REQUIRE(77 == face[1]);
    REQUIRE(44 == face[2]);
    REQUIRE(55 == face[3]);

    Parfait::CGNS::getPrismFace(cell, 2, face);
    REQUIRE(4 == face.size());
    REQUIRE(77 == face[0]);
    REQUIRE(66 == face[1]);
    REQUIRE(99 == face[2]);
    REQUIRE(44 == face[3]);

    Parfait::CGNS::getPrismFace(cell, 3, face);
    REQUIRE(3 == face.size());
    REQUIRE(99 == face[0]);
    REQUIRE(77 == face[1]);
    REQUIRE(88 == face[2]);

    Parfait::CGNS::getPrismFace(cell, 4, face);
    REQUIRE(3 == face.size());
    REQUIRE(66 == face[0]);
    REQUIRE(55 == face[1]);
    REQUIRE(44 == face[2]);
}

TEST_CASE("Get hex faces") {
    std::vector<int> cell{99, 88, 77, 66, 55, 44, 33, 22};
    std::vector<int> face;

    Parfait::CGNS::getHexFace(cell, 0, face);
    REQUIRE(4 == face.size());
    REQUIRE(99 == face[0]);
    REQUIRE(66 == face[1]);
    REQUIRE(77 == face[2]);
    REQUIRE(88 == face[3]);

    Parfait::CGNS::getHexFace(cell, 1, face);
    REQUIRE(4 == face.size());
    REQUIRE(99 == face[0]);
    REQUIRE(88 == face[1]);
    REQUIRE(44 == face[2]);
    REQUIRE(55 == face[3]);

    Parfait::CGNS::getHexFace(cell, 2, face);
    REQUIRE(4 == face.size());
    REQUIRE(88 == face[0]);
    REQUIRE(77 == face[1]);
    REQUIRE(33 == face[2]);
    REQUIRE(44 == face[3]);

    Parfait::CGNS::getHexFace(cell, 3, face);
    REQUIRE(4 == face.size());
    REQUIRE(77 == face[0]);
    REQUIRE(66 == face[1]);
    REQUIRE(22 == face[2]);
    REQUIRE(33 == face[3]);

    Parfait::CGNS::getHexFace(cell, 4, face);
    REQUIRE(4 == face.size());
    REQUIRE(99 == face[0]);
    REQUIRE(55 == face[1]);
    REQUIRE(22 == face[2]);
    REQUIRE(66 == face[3]);

    Parfait::CGNS::getHexFace(cell, 5, face);
    REQUIRE(4 == face.size());
    REQUIRE(55 == face[0]);
    REQUIRE(44 == face[1]);
    REQUIRE(33 == face[2]);
    REQUIRE(22 == face[3]);
}
