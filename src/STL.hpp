#include <assert.h>
#include <math.h>
#include "Point.h"
#include "Extent.h"
#include <stdexcept>
#include <exception>
#include "STLReader.h"
namespace Parfait {
  namespace STL {
    inline void STL::rescale(double scale) {
#pragma omp parallel for
        for (unsigned int i = 0; i < facets.size(); i++) {
            for (unsigned int j = 0; j < 3; j++) { facets[i][0][j] /= scale; }
            for (unsigned int j = 0; j < 3; j++) { facets[i][1][j] /= scale; }
            for (unsigned int j = 0; j < 3; j++) { facets[i][2][j] /= scale; }
        }
    }

    inline void STL::scaleToUnitLength() {
        auto d = findDomain();
        scaleToUnitLength(d);
    }

    inline void STL::scaleToUnitLength(const Extent &d) {
        translateCenterToOrigin(d);
        double length = getLongestCartesianLength(d);
        rescale(length);
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

    inline Parfait::Point<double> SearchSTL::getClosestPoint(const Point &p, double &dist) const {
        dist = 0.001;
        return LoopClosest(p, dist);
    }

    inline Parfait::Point<double> SearchSTL::getClosestPointWithSeed(const Point &point, double &dist) const {
        return LoopClosest(point, dist);
    }

    inline Parfait::Point<double> SearchSTL::LoopClosest(const Point &point, double &dist) const {
        // Refactor this and below so we can check if a point returned is inside the extent box.
        bool found = false;
        Point closest;
        bool done = false;
        int count = 0;
        while (!found) {
            Point offset{dist, dist, dist};
            Extent extent{point - offset, point + offset};

            auto inside = adt.retrieve(extent);
            for (int index = 0; index < inside.size(); index++) {
                auto facetIndex = inside[index];
                auto &facet = stl.facets[facetIndex];
                double distanceToFacet;
                Point closestPointOnFacet = facet.GetClosestPoint(point, distanceToFacet);
                if (distanceToFacet < dist) {
                    {
                        dist = distanceToFacet;
                        closest = closestPointOnFacet;
                        found = true;
                    }
                }
            }
            if (dist < 1.0e-16) {
                dist = 1.0e-16;
            }
            count++;
            if (count == 50) {
                fprintf(stderr,
                        "\n[ERROR]: You are clearly stuck in a loop and can't find the nearest point on the geometry.");
                fprintf(stderr, "\nSearching From Point: %lf %lf %lf", point[0], point[1], point[2]);
                throw std::logic_error("Stuck in a loop can't find geometry");
            }
            inside.clear();
            if (!found) {
                dist *= 2.0;
            } else if (!done) {
                // If this is the first candidate node we've found.
                // Make sure there aren't any closer points that we just barely missed.
                // Do this by making an extent box that includes things we would have missed
                // in a spherical extent.
                dist *= 1.7;
                done = true;
                found = false;
            }
        }

        return closest;
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
#pragma omp parallel for
        for (int facetId = 0; facetId < stl.facets.size(); facetId++) {
            auto &facet = stl.facets[facetId];
            adt.store(facetId, ExtentBuilder::build(facet));
        }
    }

    inline Facet &STL::operator[](const int i) {
        return facets[i];
    }

    inline Parfait::Point<double> SearchSTL::getClosestPoint(const Point &point) const {
        double distance;
        return getClosestPoint(point, distance);
    }
  }
}