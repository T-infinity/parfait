
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

#include <array>
#include "Point.h"
#include "Extent.h"

namespace Parfait {
  class Facet {

  public:
      Facet() = default;
      Facet(const Point<double> &p1, const Point<double> &p2, const Point<double> &p3);
      std::array<Point<double>, 3> points;
      Point<double> normal;

      Extent<double> getExtent() const;
      bool WhereDoesEdgeIntersect(const Point<double> &edgelo,
                                  const Point<double> &edgehi,
                                  Point<double> &point) const;

      bool DoesRayIntersect(const Parfait::Point<double>& orig,
                            const Parfait::Point<double>& dir,
                            double &t,
                            bool *confident = nullptr) const;

      Point<double> GetClosestPoint(Point<double> point) const;

      Point<double> &operator[](int i);
      const Point<double> &operator[](int i) const;

      int numberOfNodes() const { return 3; }

      void getNode(int i, double p[3]) const;
      Point<double> getNode(int i) const;

      double GetXMin();
      double GetYMin();
      double GetZMin();

      double GetXMax();
      double GetYMax();
      double GetZMax();
      Point<double> computeNormal() const;

      static double getLargestAngleBetween(const std::vector<Facet> &facets);

      bool intersects(const Extent<double> &e) const;
  private:
      double GetMax(int i);
      double GetMin(int i);
      bool triBoxOverlap(const Parfait::Point<double>&, const Parfait::Point<double>& boxhalfsize,
                         const std::array<Parfait::Point<double>, 3>& verts) const;
  };
}

#include "Facet.hpp"