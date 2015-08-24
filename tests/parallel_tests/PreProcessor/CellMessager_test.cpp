#include <CellMessager.h>
#include <vector>
#include <CppUTest/CommandLineTestRunner.h>

TEST_GROUP(CellMessagerTests){};

TEST(CellMessagerTests, Exists){
    std::vector<int> triangles;
    std::vector<int> triangleTags;
    CellMessager cellMessager(triangles, triangleTags);

    cellMessager.sendTriangles({0,1,2,3,-1});
    LONGS_EQUAL(1, triangles.size() / 3);
    cellMessager.sendTriangles({0,1,2,3,-1,
                                4,5,6,7,-1});
    LONGS_EQUAL(3, triangles.size() / 3);
}

