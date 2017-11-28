#include <assert.h>
#include <math.h>
#include "Point.h"
#include "Extent.h"
#include <stdexcept>
#include <exception>
#include "STLReader.h"

namespace Parfait {
namespace STL {

template <typename P>
void STL::addFacet(const P& p1, const P& p2, const P& p3){
    facets.push_back({{p1[0], p1[1], p1[2]},
                      {p2[0], p2[1], p2[2]},
                      {p3[0], p3[1], p3[2]}});
}

inline void STL::rescale(double scale) {
    for (unsigned int i = 0; i < facets.size(); i++) {
        for (unsigned int j = 0; j < 3; j++) { facets[i][0][j] *= scale; }
        for (unsigned int j = 0; j < 3; j++) { facets[i][1][j] *= scale; }
        for (unsigned int j = 0; j < 3; j++) { facets[i][2][j] *= scale; }
    }
}

inline void STL::scaleToUnitLength() {
    auto d = findDomain();
    scaleToUnitLength(d);
}

inline void STL::scaleToUnitLength(const Extent &d) {
    translateCenterToOrigin(d);
    double length = getLongestCartesianLength(d);
    rescale(1.0/length);
}

inline double STL::getLongestCartesianLength(const Extent &d){
    double dx = d.hi[0] - d.lo[0];
    double dy = d.hi[1] - d.lo[1];
    double dz = d.hi[2] - d.lo[2];
    double max = -20e20;
    max = (max > dx) ? (max) : (dx);
    max = (max > dy) ? (max) : (dy);
    max = (max > dz) ? (max) : (dz);
    return max;
}

inline double STL::getLongestCartesianLength() {
    Extent domain = findDomain();
    return getLongestCartesianLength(domain);
}

inline void STL::translateCenterToOrigin(const Extent &d) {
    double x_mid = 0.5 * (d.lo[0] + d.hi[0]);
    double y_mid = 0.5 * (d.lo[1] + d.hi[1]);
    double z_mid = 0.5 * (d.lo[2] + d.hi[2]);

    for(auto &f : facets){
        for(int i = 0; i < 3; i++){
            f[i][0] -= x_mid;
            f[i][1] -= y_mid;
            f[i][2] -= z_mid;
        }
    }
}

inline void STL::translateCenterToOrigin() {

    auto d = findDomain();
    return translateCenterToOrigin(d);
}

inline void STL::loadBinaryFile(std::string fileName) {

    Reader reader(fileName);
    facets = reader.readFacets();
}

inline void STL::writeAsciiFile(std::string fname, std::string solidName) const {

    fname += ".stl";
    FILE *fp = fopen(fname.c_str(), "w");
    assert(fp != NULL);
    printf("\nWriting stl to <%s>", fname.c_str());
    fflush(stdout);

    fprintf(fp, "solid %s", solidName.c_str());
    for (unsigned int i = 0; i < facets.size(); i++) {
        fprintf(fp, "\nfacet normal %lf %lf %lf", facets[i].normal[0], facets[i].normal[1], facets[i].normal[2]);
        fprintf(fp, "\nouter loop");
        fprintf(fp, "\nvertex %lf %lf %lf", facets[i][0][0], facets[i][0][1], facets[i][0][2]);
        fprintf(fp, "\nvertex %lf %lf %lf", facets[i][1][0], facets[i][1][1], facets[i][1][2]);
        fprintf(fp, "\nvertex %lf %lf %lf", facets[i][2][0], facets[i][2][1], facets[i][2][2]);
        fprintf(fp, "\nendloop\nendfacet");
    }
    fprintf(fp, "\nendsolid %s", solidName.c_str());

    fclose(fp);
}

inline Parfait::Extent<double> STL::findDomain() const {
    Extent domain(Point(20e20, 20e20, 20e20), Point(-20e20, -20e20, -20e20));

    for (auto &facet : facets) {
        for (int i = 0; i < 3; i++) {
            domain.lo[i] = (domain.lo[i] < facet[0][i]) ? (domain.lo[i]) : (facet[0][i]);
            domain.lo[i] = (domain.lo[i] < facet[1][i]) ? (domain.lo[i]) : (facet[1][i]);
            domain.lo[i] = (domain.lo[i] < facet[2][i]) ? (domain.lo[i]) : (facet[2][i]);

            domain.hi[i] = (domain.hi[i] > facet[0][i]) ? (domain.hi[i]) : (facet[0][i]);
            domain.hi[i] = (domain.hi[i] > facet[1][i]) ? (domain.hi[i]) : (facet[1][i]);
            domain.hi[i] = (domain.hi[i] > facet[2][i]) ? (domain.hi[i]) : (facet[2][i]);
        }
    }
    return domain;
}

inline Parfait::Point<double> SearchSTL::getClosestPoint(const Point &p) const {
    double dist = 0.001;
    return LoopClosest(p, dist);
}

inline Parfait::Point<double> SearchSTL::getClosestPointWithSeed(const Point &point, double &dist) const {
    return LoopClosest(point, dist);
}

inline Parfait::Point<double> SearchSTL::getClosestPointToFacets(const std::vector<int>& facet_indices, const Point& point) const {
    double dist = std::numeric_limits<double>::max();
    Parfait::Point<double> closest;
    for (unsigned int index = 0; index < facet_indices.size(); index++) {
        auto facetIndex = facet_indices[index];
        auto &facet = stl.facets[facetIndex];
        double distanceToFacet;
        auto candidate_closer = facet.GetClosestPoint(point, distanceToFacet);
        if (distanceToFacet < dist) {
            dist = distanceToFacet;
            closest = candidate_closer;
        }
    }
    return closest;
}

inline Parfait::Point<double> SearchSTL::LoopClosest(const Point &point, double &dist) const {
    Point closest;
    for(int loop = 0; loop < 500; loop++){
        Point offset{dist, dist, dist};
        Extent extent{point - offset, point + offset};

        auto inside = adt.retrieve(extent);
        if(inside.size() != 0){
            dist *= 1.7;
            Point offset{dist, dist, dist};
            Extent extent{point - offset, point + offset};
            inside =  adt.retrieve(extent);
            return getClosestPointToFacets(inside, point);
        }
        dist *= 2.0;
    }

    fprintf(stderr, "\n[ERROR]: You are clearly stuck in a loop and can't find the nearest point on the geometry.");
    fprintf(stderr, "\nSearching From Point: %lf %lf %lf", point[0], point[1], point[2]);
    throw std::logic_error("Stuck in a loop can't find geometry");
}

inline std::vector<Facet> SearchSTL::getFacetsInsideExtent(const Extent &domain) const {

    auto ids = adt.retrieve(domain);
    fflush(stdout);
    std::vector<Facet> inside;
    for (auto &id : ids) {
        inside.push_back(stl.facets[id]);
    }
    return inside;
}

inline SearchSTL::SearchSTL(const STL &stl_in) : stl(stl_in), adt(stl_in.findDomain()) {
//#pragma omp parallel for
    for (unsigned int facetId = 0; facetId < stl.facets.size(); facetId++) {
        auto &facet = stl.facets[facetId];
        adt.store(facetId, ExtentBuilder::build(facet));
    }
}

inline Facet &STL::operator[](const int i) {
    return facets[i];
}
}
}