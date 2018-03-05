
// Copyright 2016 United States Government as represented by the Administrator of the National Aeronautics and Space Administration. 
// No copyright is claimed in the United States under Title 17, U.S. Code. All Other Rights Reserved.
// 
// The “Parfait: A Toolbox for CFD Software Development [LAR-18839-1]” platform is licensed under the 
// Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. 
// You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0. 
// 
// Unless required by applicable law or agreed to in writing, software distributed under the License is distributed 
// on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. 
// See the License for the specific language governing permissions and limitations under the License.
#pragma once

#include <string>
#include <vector>
#include <stdint.h>
#include "Point.h"
#include "Facet.h"
#include "ExtentBuilder.h"
#include "Adt3dExtent.h"

namespace Parfait {
  namespace STL {
    class STL {
        typedef Parfait::Extent<double> Extent;
        typedef Parfait::Point<double> Point;
    public:
        STL() = default;
        STL(const std::vector<Facet>& facets):facets(facets){}
        template <typename P>
        void addFacet(const P& p1, const P& p2, const P& p3);

        std::string filename;
        std::vector<Facet> facets;

        Facet &operator[](const int i);

        int size() { return (int) facets.size(); }

        void loadBinaryFile(std::string fileName);
        void writeAsciiFile(std::string fileName, std::string solidName = "solidName") const;
        void translateCenterToOrigin();
        void translateCenterToOrigin(const Extent &d);
        void scaleToUnitLength();
        void scaleToUnitLength(const Extent &d);
        void rescale(double scale);
        double getLongestCartesianLength();
        double getLongestCartesianLength(const Extent &d);
        Extent findDomain() const;
    };

    class SearchSTL {
        typedef Parfait::Extent<double> Extent;
        typedef Parfait::Point<double> Point;
    public:
        SearchSTL(const STL &stl_in);

        Parfait::Point<double> getClosestPoint(const Point &point) const;
        Point getClosestPointWithSeed(const Point &point, double dist) const;
        std::vector<Facet> getFacetsInsideExtent(const Extent &e) const;
        Parfait::Point<double> getClosestPointToFacets(const std::vector<int>& facet_indices, const Point& point) const;

        Extent findDomain() const { return stl.findDomain(); }

    private:
        const STL &stl;
        Parfait::Adt3DExtent adt;
        Point LoopClosest(const Point &query_point, double search_radius) const;
    };
  }
}

#include "STL.hpp"