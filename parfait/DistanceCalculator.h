
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
#include <vector>
#include "STL.h"
#include "Point.h"
#include <memory>
#include <MessagePasser/MessagePasser.h>
#include <parfait/TreeDist.h>

namespace Parfait {
  class DistanceCalculator {
  public:

      enum { TRI_3, QUAD_4 };

      template<typename FillPoint, typename FillCell, typename CellType>
      DistanceCalculator(MessagePasser mp,
                         FillPoint fillPoint,
                         FillCell fillCell,
                         CellType cellType,
                         int cell_count)
          : mp(mp), tree(buildTree(extractFacets(fillPoint, fillCell, cellType, cell_count))) {
      };

      inline Parfait::Point<double> closest(const Parfait::Point<double> &p) const {
          return tree.closestPoint(p);
      }

      double distance(const Parfait::Point<double> &p) const {
          return (closest(p) - p).magnitude();
      }

  private:
      MessagePasser mp;
      Parfait::DistanceTree tree;

      template<typename FillPoint, typename FillCell, typename CellType>
      std::vector<Parfait::Facet> extractFacets(FillPoint, FillCell, CellType, int);

      Parfait::DistanceTree buildTree(const std::vector<Parfait::Facet> &facets) {
          auto e = Parfait::ExtentBuilder::createEmptyBuildableExtent<double>();
          for (const auto &f : facets) {
              for (int i = 0; i < 3; i++)
                  Parfait::ExtentBuilder::addPointToExtent(e, f[i]);
          }
          Parfait::DistanceTree tree(e);
          for (const auto &f : facets) {
              tree.insert(f);
          }

          tree.pruneEmpty();
          tree.contractExtents();

          return tree;
      }
  };

  template<typename FillPoint, typename FillCell>
  void appendTriFacet(std::vector<Parfait::Facet> &facets, FillPoint fillPoint, FillCell fillCell,
                      int cell_id) {

      std::array<int, 3> cell;
      fillCell(cell_id, cell.data());
      Parfait::Facet f;
      int i = 0;
      for (int n : cell) {
          fillPoint(n, f[i++].data());
      }

      facets.push_back(f);
  }
  template<typename FillPoint, typename FillCell>
  void appendQuadFacets(std::vector<Parfait::Facet> &facets, FillPoint fillPoint, FillCell fillCell,
                        int cell_id) {
      std::array<int, 4> cell;
      fillCell(cell_id, cell.data());
      Parfait::Facet f1;
      Parfait::Facet f2;

      std::array<Parfait::Point<double>, 4> p;
      int i = 0;
      for (int n : cell) {
          fillPoint(n, p[i++].data());
      }

      facets.push_back({p[0], p[1], p[2]});
      facets.push_back({p[2], p[3], p[0]});

  }

  template<typename FillPoint, typename FillCell, typename CellType>
  std::vector<Parfait::Facet> DistanceCalculator::extractFacets(FillPoint fillPoint,
                                                                FillCell fillCell,
                                                                CellType cellType,
                                                                int cell_count) {

      std::vector<Parfait::Facet> send_facets;
      std::vector<Parfait::Facet> facets;
      for (int cell_id = 0; cell_id < cell_count; cell_id++) {
          auto type = cellType(cell_id);

          if (type == DistanceCalculator::TRI_3)
              appendTriFacet(send_facets, fillPoint, fillCell, cell_id);
          if (type == DistanceCalculator::QUAD_4)
              appendQuadFacets(send_facets, fillPoint, fillCell, cell_id);
      }
      mp.AllGatherv(send_facets, facets);
      return facets;
  };

}
