#include <catch.hpp>
#include <parfait/Facet.h>
#include <limits>
#include <parfait/TreeDist.h>

TEST_CASE("Sphere extent intersection"){
    Parfait::Extent<double> e = {{100,100,100}, {101,101,101}};
    Parfait::Point<double> center = {99, 99, 99};
    double radius = 2.0;

    REQUIRE(Parfait::extentSphereIntersection(e, center, radius));
    REQUIRE(Parfait::extentSphereIntersection({{0,0,0},{1,0,0}}, {10,0,0}, 11));
}

TEST_CASE("DistanceTree::Node::isLeaf "){
    Parfait::Extent<double> e = {{0,0,0}, {1,1,1}};
    int depth = 0;
    Parfait::DistanceTree::Node node(e, depth);
    REQUIRE(node.isLeaf());
}

TEST_CASE("DistanceTree::Node::childrenExtent(i)"){
    Parfait::Extent<double> extent = {{0,0,0}, {1,1,1}};
    int depth = 0;
    Parfait::DistanceTree::Node node(extent, depth);
    SECTION("child 0"){
        auto e = node.childExtent(0);
        REQUIRE(e.lo[0] == Approx(0));
        REQUIRE(e.lo[1] == Approx(0));
        REQUIRE(e.lo[2] == Approx(0));
        REQUIRE(e.hi[0] == Approx(0.5));
        REQUIRE(e.hi[1] == Approx(0.5));
        REQUIRE(e.hi[2] == Approx(0.5));
    }
    SECTION("child 1"){
        auto e = node.childExtent(1);
        REQUIRE(e.lo[0] == Approx(0.5));
        REQUIRE(e.lo[1] == Approx(0));
        REQUIRE(e.lo[2] == Approx(0));
        REQUIRE(e.hi[0] == Approx(1.0));
        REQUIRE(e.hi[1] == Approx(0.5));
        REQUIRE(e.hi[2] == Approx(0.5));
    }
    SECTION("child 2"){
        auto e = node.childExtent(2);
        REQUIRE(e.lo[0] == Approx(0.5));
        REQUIRE(e.lo[1] == Approx(0.5));
        REQUIRE(e.lo[2] == Approx(0));
        REQUIRE(e.hi[0] == Approx(1.0));
        REQUIRE(e.hi[1] == Approx(1.0));
        REQUIRE(e.hi[2] == Approx(0.5));
    }
    SECTION("child 3"){
        auto e = node.childExtent(3);
        REQUIRE(e.lo[0] == Approx(0));
        REQUIRE(e.lo[1] == Approx(0.5));
        REQUIRE(e.lo[2] == Approx(0));
        REQUIRE(e.hi[0] == Approx(0.5));
        REQUIRE(e.hi[1] == Approx(1.0));
        REQUIRE(e.hi[2] == Approx(0.5));
    }
    SECTION("child 4"){
        auto e = node.childExtent(4);
        REQUIRE(e.lo[0] == Approx(0));
        REQUIRE(e.lo[1] == Approx(0));
        REQUIRE(e.lo[2] == Approx(0.5));
        REQUIRE(e.hi[0] == Approx(0.5));
        REQUIRE(e.hi[1] == Approx(0.5));
        REQUIRE(e.hi[2] == Approx(1.0));
    }
    SECTION("child 5"){
        auto e = node.childExtent(5);
        REQUIRE(e.lo[0] == Approx(0.5));
        REQUIRE(e.lo[1] == Approx(0));
        REQUIRE(e.lo[2] == Approx(0.5));
        REQUIRE(e.hi[0] == Approx(1.0));
        REQUIRE(e.hi[1] == Approx(0.5));
        REQUIRE(e.hi[2] == Approx(1.0));
    }
    SECTION("child 6"){
        auto e = node.childExtent(6);
        REQUIRE(e.lo[0] == Approx(0.5));
        REQUIRE(e.lo[1] == Approx(0.5));
        REQUIRE(e.lo[2] == Approx(0.5));
        REQUIRE(e.hi[0] == Approx(1.0));
        REQUIRE(e.hi[1] == Approx(1.0));
        REQUIRE(e.hi[2] == Approx(1.0));
    }
    SECTION("child 7"){
        auto e = node.childExtent(7);
        REQUIRE(e.lo[0] == Approx(0));
        REQUIRE(e.lo[1] == Approx(0.5));
        REQUIRE(e.lo[2] == Approx(0.5));
        REQUIRE(e.hi[0] == Approx(0.5));
        REQUIRE(e.hi[1] == Approx(1.0));
        REQUIRE(e.hi[2] == Approx(1.0));
    }
}

TEST_CASE("DistanceTree Requires domain"){
    Parfait::Extent<double> e{{0,0,0}, {1,1,1}};
    Parfait::DistanceTree tree(e);
    tree.setMaxDepth(8);
    Parfait::Facet f = {{0,0,0}, {1,0,0}, {1,1,1}};
    tree.insert(f);

    auto p = tree.closestPoint({10,10,10});
    REQUIRE(p[0] == Approx(1.0));
    REQUIRE(p[1] == Approx(1.0));
    REQUIRE(p[2] == Approx(1.0));
}

TEST_CASE("DistanceTree can store multiple facets") {
    Parfait::Extent<double> e{{0,0,0}, {1,1,1}};
    Parfait::DistanceTree tree(e);
    tree.setMaxDepth(8);
    Parfait::Facet f = {{0, 0, 0}, {0.5, 0, 0}, {.5, .5, .5}};
    tree.insert(f);
    f = {{0.5, 0.5, 0.5}, {1, .5, .5}, {1, 1, 1}};
    tree.insert(f);

    auto p = tree.closestPoint({10, 10, 10});
    REQUIRE(p[0] == Approx(1.0));
    REQUIRE(p[1] == Approx(1.0));
    REQUIRE(p[2] == Approx(1.0));
}

TEST_CASE("Closest point in AABB"){
    Parfait::Extent<double> e{{10,10,10},{11,11,11}};
    Parfait::Point<double> query{10.5,10.5,22};

    SECTION("snap z"){
        auto p = Parfait::getClosestPointInExtent(e, {10.5, 10.5, 22});
        REQUIRE(p[0] == Approx(10.5));
        REQUIRE(p[1] == Approx(10.5));
        REQUIRE(p[2] == Approx(11));
    }
    SECTION("snap upper-corner"){
        auto p = Parfait::getClosestPointInExtent(e, {22, 22, 22});
        REQUIRE(p[0] == Approx(11));
        REQUIRE(p[1] == Approx(11));
        REQUIRE(p[2] == Approx(11));
    }
    SECTION("snap lower-corner"){
        auto p = Parfait::getClosestPointInExtent(e, {2, 2, 2});
        REQUIRE(p[0] == Approx(10));
        REQUIRE(p[1] == Approx(10));
        REQUIRE(p[2] == Approx(10));
    }
    SECTION("point inside voxel"){
        auto p = Parfait::getClosestPointInExtent(e, {10.5, 10.5, 10.7});
        REQUIRE(p[0] == Approx(10.5));
        REQUIRE(p[1] == Approx(10.5));
        REQUIRE(p[2] == Approx(10.7));
    }
}

TEST_CASE("DistanceTree shrink extents"){
    Parfait::Extent<double> e{{0,0,0}, {1,1,1}};
    Parfait::DistanceTree tree(e);
    tree.setMaxDepth(8);
    Parfait::Facet f = {{0, 0, 0}, {1, 0, 0}, {1, 1, 0.0}};
    tree.insert(f);
    f = {{0, 0, 1}, {1, 0, 1}, {1, 1, 1}};
    tree.insert(f);

    tree.contractExtents();

    auto p = tree.closestPoint({10, 10, 10});
    REQUIRE(p[0] == Approx(1));
    REQUIRE(p[1] == Approx(1));
    REQUIRE(p[2] == Approx(1));

    p = tree.closestPoint({0, 0, 0});
    REQUIRE(p[0] == Approx(0));
    REQUIRE(p[1] == Approx(0));
    REQUIRE(p[2] == Approx(0));
}
