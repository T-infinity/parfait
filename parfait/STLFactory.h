
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

#include "STL.h"
namespace Parfait {

  namespace STLFactory {
    STL::STL getUnitCube();
    STL::STL getLShapedSurface();
  };

  inline STL::STL STLFactory::getLShapedSurface() {
      STL::STL stl;
      std::vector <Point<double>> points(6);
      points[0] = {0, 0, 0};
      points[1] = {1, 0, 0};
      points[2] = {1, 1, 0};
      points[3] = {0, 1, 0};
      points[4] = {1, 0, 1};
      points[5] = {1, 1, 1};

      stl.facets.push_back({points[0], points[1], points[2]});
      stl.facets.push_back({points[0], points[2], points[3]});
      stl.facets.push_back({points[1], points[4], points[5]});
      stl.facets.push_back({points[1], points[5], points[2]});
      return stl;
  }

  inline STL::STL STLFactory::getUnitCube() {
      STL::STL cube;

      Facet bottom1;
      bottom1[0] = {0, 0, 0};
      bottom1[1] = {1, 1, 0};
      bottom1[2] = {1, 0, 0};
      cube.facets.push_back(bottom1);

      Facet bottom2;
      bottom2[0] = {0, 0, 0};
      bottom2[1] = {0, 1, 0};
      bottom2[2] = {1, 1, 0};
      cube.facets.push_back(bottom2);

      Facet front1;
      front1[0] = {0, 0, 0};
      front1[1] = {1, 0, 0};
      front1[2] = {1, 0, 1};
      cube.facets.push_back(front1);

      Facet front2;
      front2[0] = {0, 0, 0};
      front2[1] = {1, 0, 1};
      front2[2] = {0, 0, 1};
      cube.facets.push_back(front2);

      Facet right1;
      right1[0] = {1, 0, 0};
      right1[1] = {1, 1, 0};
      right1[2] = {1, 1, 1};
      cube.facets.push_back(right1);

      Facet right2;
      right2[0] = {1, 0, 0};
      right2[1] = {1, 1, 1};
      right2[2] = {1, 0, 1};
      cube.facets.push_back(right2);

      Facet back1;
      back1[0] = {0, 1, 0};
      back1[1] = {0, 1, 1};
      back1[2] = {1, 1, 1};
      cube.facets.push_back(back1);

      Facet back2;
      back2[0] = {0, 1, 0};
      back2[1] = {1, 1, 1};
      back2[2] = {1, 1, 0};
      cube.facets.push_back(back2);

      Facet left1;
      left1[0] = {0, 0, 0};
      left1[1] = {0, 1, 1};
      left1[2] = {0, 1, 0};
      cube.facets.push_back(left1);

      Facet left2;
      left2[0] = {0, 0, 0};
      left2[1] = {0, 0, 1};
      left2[2] = {0, 1, 1};
      cube.facets.push_back(left2);

      Facet top1;
      top1[0] = {0, 0, 1};
      top1[1] = {1, 0, 1};
      top1[2] = {1, 1, 1};
      cube.facets.push_back(top1);

      Facet top2;
      top2[0] = {0, 0, 1};
      top2[1] = {1, 1, 1};
      top2[2] = {0, 1, 1};
      cube.facets.push_back(top2);


      return cube;
  }
}