#include <catch.hpp>
#include <parfait/Facet.h>

class OctTree {
    class Node {
    public:
        enum {EMPTY = -11, ROOT = -88};
        Parfait::Extent<double> extent = {{0,0,0}, {1,1,1}};
        int parent = EMPTY;
        std::array<int, 8> children {
            EMPTY, EMPTY, EMPTY, EMPTY,
            EMPTY, EMPTY, EMPTY, EMPTY
        };

        std::vector<int> inside;
    };
public:
    void insert(const Parfait::Facet& f, int id){

    }
    void setMaxDepth(int depth){
        max_depth = depth;
    }

    std::vector<int> retrieve(const Parfait::Point<double>& p, double radius){
        std::vector<int> inside;

        return inside;
    }
private:
    int max_depth = 11;
};


bool extentSphereIntersection(const Parfait::Extent<double>& e, const Parfait::Point<double>& center, const double radius){
    double r2 = radius * radius;
    double dmin = 0;
    for( int i = 0; i < 3; i++ ) {
        if( center[i] < e.lo[i] ) dmin += std::pow((center[i] - e.lo[i]), 2);
        else if( center[i] > e.hi[i] ) dmin += std::pow((center[i] - e.hi[i]), 2);
    }
    return dmin <= r2;
}

TEST_CASE("Sphere extent intersection"){
    Parfait::Extent<double> e = {{100,100,100}, {101,101,101}};
    Parfait::Point<double> center = {99, 99, 99};
    double radius = 2.0;

    REQUIRE(extentSphereIntersection(e, center, radius));
}

TEST_CASE("OctTree Exists"){
    OctTree tree;
    tree.setMaxDepth(8);
    Parfait::Facet f = {{0,0,0}, {1,0,0}, {1,1,1}};
    tree.insert(f, 7);

    auto inside = tree.retrieve({0,0,0}, 1.0);
    REQUIRE(inside.size() == 1);
}
