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

        std::vector<Parfait::Facet> inside_facets;
    };
public:
    void initializeRoot(const Parfait::Facet& f) {
        auto root = Node();
        root.parent = Node::ROOT;
        root.extent = root_extent;
        root.inside_facets.push_back(f);
        nodes.push_back(root);
    }
    void insert(const Parfait::Facet& f){
        if(nodes.size() == 0)
            initializeRoot(f);
    }
    void setMaxDepth(int depth){
        max_depth = depth;
    }

    Parfait::Point<double> closestPoint(const Parfait::Point<double>& p){
        auto& f = nodes[0].inside_facets[0];
        return f.GetClosestPoint(p);
    }
private:
    int max_depth = 11;
    Parfait::Extent<double> root_extent = {{0,0,0}, {1,1,1}};
    std::vector<Node> nodes;
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
    tree.insert(f);

    auto p = tree.closestPoint({10,10,10});
    REQUIRE(p[0] == Approx(1.0));
    REQUIRE(p[1] == Approx(1.0));
    REQUIRE(p[2] == Approx(1.0));
}
