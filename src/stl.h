#ifndef STL_H
#define STL_H

#include <string>
#include <vector>
#include <stdint.h>
#include "Point.h"
#include "Facet.h"
#include "ExtentBuilder.h"
#include "Adt3dExtent.h"

namespace Parfait {
    class STL {
        typedef Parfait::Extent<double> Extent;
        typedef Parfait::Point<double> Point;
    public:
        STL() = default;

        std::string filename;
        std::vector<Facet> facets;

        Facet &operator[](const int i);

        int size() { return (int) facets.size(); }

        void loadBinaryFile(std::string fileName);
        void writeAsciiFile(std::string fileName, std::string solidName = "solidName") const;
        void translateCenterToOrigin();
        void scaleToUnitLength();
        void rescale(double scale);
        double getLongestCartesianLength();
        Extent findDomain() const;
    };

    class SearchSTL {
        typedef Parfait::Extent<double> Extent;
        typedef Parfait::Point<double> Point;
    public:
        SearchSTL(const STL &stl_in);

        Point getClosestPoint(const Point &point, double &dist) const;
        Point getClosestPoint(const Point &point) const;
        Point getClosestPointWithSeed(const Point &point, double &dist) const;
        std::vector<Facet> getFacetsInsideExtent(const Extent &e) const;


        Extent findDomain() const { return stl.findDomain(); }

    private:
        const STL &stl;
        Parfait::Adt3DExtent adt;
        Point LoopClosest(const Point &point, double &dist) const;
    };
}

#include "stl.hpp"

#endif

