
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

namespace Parfait {
  namespace CGNS {
    namespace Tet {
      constexpr int numberOfNodes() {
          return 4;
      }
      constexpr int numberOfEdges() {
          return 6;
      }
      static constexpr std::array<std::array<int, 2>, 6> edge_to_node =
              {{
                       {0, 1},
                       {1, 2},
                       {2, 0},
                       {0, 3},
                       {1, 3},
                       {2, 3}
               }};

      static constexpr std::array<std::array<int, 2>, 6> edge_to_face =
              {{
                       {0, 1},
                       {0, 2},
                       {0, 3},
                       {1, 3},
                       {2, 1},
                       {3, 2}
               }};

      template<typename Tet>
      Parfait::Point<double> computeCenter(const Tet& tet){
          return 0.25*(tet[0] + tet[1] + tet[2] + tet[3]);
      }

      template<typename Tet>
      std::array<Parfait::Point<double>, 6> computeEdgeCenters(const Tet& tet) {
          std::array<Parfait::Point<double>, 6> edge_centers;
          edge_centers[0] = 0.5 * (tet[0] + tet[1]);
          edge_centers[1] = 0.5 * (tet[1] + tet[2]);
          edge_centers[2] = 0.5 * (tet[2] + tet[0]);
          edge_centers[3] = 0.5 * (tet[0] + tet[3]);
          edge_centers[4] = 0.5 * (tet[1] + tet[3]);
          edge_centers[5] = 0.5 * (tet[2] + tet[3]);
          return edge_centers;
      }

      template<typename Tet>
      std::array<Parfait::Point<double>, 4> computeFaceCenters(const Tet& tet) {
          std::array<Parfait::Point<double>, 4> face_centers;
          double third = 1.0 / 3.0;
          face_centers[0] = third * (tet[0] + tet[1] + tet[2]);
          face_centers[1] = third * (tet[0] + tet[1] + tet[3]);
          face_centers[2] = third * (tet[3] + tet[1] + tet[2]);
          face_centers[3] = third * (tet[0] + tet[2] + tet[3]);
          return face_centers;
      }
    }
    namespace Pyramid {
      constexpr int numberOfEdges() {
          return 8;
      }
      constexpr static std::array<std::array<int,2>,8> edge_to_node
              {{
                       {0,1},
                       {1,2},
                       {2,3},
                       {3,0},
                       {0,4},
                       {1,4},
                       {2,4},
                       {3,4},
               }};
      constexpr static std::array<std::array<int,2>,8> edge_to_face
              {{
                       {0,1},
                       {0,2},
                       {0,3},
                       {0,4},
                       {1,4},
                       {2,1},
                       {3,2},
                       {4,3}
               }};
      template<typename Pyramid>
      Parfait::Point<double> computeCenter(const Pyramid& pyramid){
          return 0.25*(pyramid[0] + pyramid[1] + pyramid[2] + pyramid[3] + pyramid[4]);
      }

      template<typename Pyramid>
      std::array<Parfait::Point<double>, 8> computeEdgeCenters(const Pyramid& pyramid) {
          std::array<Parfait::Point<double>, 8> edge_centers;
          for(int i = 0; i < 8; i++)
              edge_centers[i] = 0.5*(pyramid[edge_to_node[i][0]] + pyramid[edge_to_node[i][1]]);
          return edge_centers;
      }
      template<typename Pyramid>
      std::array<Parfait::Point<double>, 5> computeFaceCenters(const Pyramid& pyramid) {
          std::array<Parfait::Point<double>, 5> face_centers;
          double third = 1.0 / 3.0;
          face_centers[0] = 0.25 * (pyramid[0] + pyramid[1] + pyramid[2] + pyramid[3]);
          face_centers[1] = third * (pyramid[0] + pyramid[1] + pyramid[4]);
          face_centers[2] = third * (pyramid[1] + pyramid[2] + pyramid[4]);
          face_centers[3] = third * (pyramid[2] + pyramid[3] + pyramid[4]);
          face_centers[4] = third * (pyramid[3] + pyramid[0] + pyramid[4]);
          return face_centers;
      }
    }
    namespace Prism {
      constexpr int numberOfEdges() {
          return 9;
      }
      constexpr static std::array<std::array<int,2>,9> edge_to_node
              {{
                       {0,1},
                       {1,2},
                       {2,0},
                       {0,3},
                       {1,4},
                       {2,5},
                       {3,4},
                       {4,5},
                       {5,3}
               }};
      constexpr static std::array<std::array<int,2>,9> edge_to_face
              {{
                       {3,0},
                       {3,1},
                       {3,2},
                       {0,2},
                       {1,0},
                       {2,1},
                       {0,4},
                       {1,4},
                       {2,4}
               }};

      template<typename Prism>
      Parfait::Point<double> computeCenter(const Prism& prism){
          return 1.0/6.0*(prism[0] + prism[1] + prism[2] +
                  prism[3]+ prism[4] + prism[5]);
      }

      template<typename Prism>
      std::array<Parfait::Point<double>, 9> computeEdgeCenters(const Prism& prism) {
          std::array<Parfait::Point<double>, 9> edge_centers;
          for(int i = 0; i < 9; i++)
              edge_centers[i] = 0.5*(prism[edge_to_node[i][0]] + prism[edge_to_node[i][1]]);
          return edge_centers;
      }

      template<typename Prism>
      std::array<Parfait::Point<double>, 5> computeFaceCenters(const Prism& prism) {
          std::array<Parfait::Point<double>, 5> face_centers;
          double third = 1.0 / 3.0;
          face_centers[0] = 0.25 * (prism[0] + prism[1] + prism[3] + prism[4]);
          face_centers[1] = 0.25 * (prism[1] + prism[2] + prism[4] + prism[5]);
          face_centers[2] = 0.25 * (prism[2] + prism[0] + prism[5] + prism[3]);
          face_centers[3] = third * (prism[0] + prism[1] + prism[2]);
          face_centers[4] = third * (prism[3] + prism[4] + prism[5]);
          return face_centers;
      }
    }
    namespace Hex {
      constexpr int numberOfEdges() {
          return 12;
      }
      constexpr static std::array<std::array<int,2>, 12> edge_to_node
              {{
                       {0,1},
                       {1,2},
                       {2,3},
                       {3,0},
                       {0,4},
                       {1,5},
                       {2,6},
                       {3,7},
                       {4,5},
                       {5,6},
                       {6,7},
                       {7,4},
               }};
      constexpr static std::array<std::array<int,2>, 12> edge_to_face
              {{
                       {0,1},
                       {0,2},
                       {0,3},
                       {0,4},
                       {1,4},
                       {2,1},
                       {3,2},
                       {4,3},
                       {1,5},
                       {2,5},
                       {3,5},
                       {4,5},
               }};
      template<typename Hex>
      Parfait::Point<double> computeCenter(const Hex& hex){
          return 1.0/8.0*(hex[0] + hex[1] + hex[2] + hex[3]
                          + hex[4] + hex[5] + hex[6] + hex[7]);
      }

      template<typename Hex>
      std::array<Parfait::Point<double>, 12> computeEdgeCenters(const Hex& hex) {
          std::array<Parfait::Point<double>, 12> edge_centers;
          for(int i = 0; i < 12; i++)
              edge_centers[i] = 0.5*(hex[edge_to_node[i][0]] + hex[edge_to_node[i][1]]);
          return edge_centers;
      }
      template<typename Hex>
      std::array<Parfait::Point<double>, 6> computeFaceCenters(const Hex& hex) {
          std::array<Parfait::Point<double>, 6> face_centers;
          face_centers[0] = 0.25 * (hex[0] + hex[1] + hex[2] + hex[3]);
          face_centers[1] = 0.25 * (hex[0] + hex[1] + hex[4] + hex[5]);
          face_centers[2] = 0.25 * (hex[1] + hex[2] + hex[5] + hex[6]);
          face_centers[3] = 0.25 * (hex[2] + hex[3] + hex[6] + hex[7]);
          face_centers[4] = 0.25 * (hex[0] + hex[3] + hex[4] + hex[7]);
          face_centers[5] = 0.25 * (hex[4] + hex[5] + hex[6] + hex[7]);
          return face_centers;
      }
    }
  }
}
