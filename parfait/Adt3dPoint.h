
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
#include "Adt.h"
#include "Point.h"
#include "Extent.h"
#include <string>
#include <stdexcept>

namespace Parfait {
  class Adt3DPoint {
  public:
      Adt3DPoint(const Extent<double> &domain);
      void store(int tag, const Point<double> &point);
      std::vector<int> retrieve(const Extent<double> &domain) const;
      void removeFirst(int tag, const Extent<double>& domain);

      bool isPointInAdt(const Point<double> &p, int &tag, double tol = 1.0e-8) const;
      int getNodeIdOrAddPoint(const Point<double>& p, int tag);
      int getNodeId(const Point<double>& p);

  private:
      UnitTransformer unitTransformer;
      Adt<3> adt;
  };
}

#include "Adt3dPoint.hpp"
