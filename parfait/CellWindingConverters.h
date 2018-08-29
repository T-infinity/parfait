
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

#include <functional>

namespace Parfait {

  class CGNSToAflr {
  public:
      template<typename T>
      static void convertTet(T* tet) { }
      template<typename T>
      static void convertPrism(T* prism) { }
      template<typename T>
      static void convertHex(T* hex) { }

      template<typename T>
      static void convertPyramid(T* pyramid) {
          std::swap(pyramid[2], pyramid[4]);
          std::swap(pyramid[1], pyramid[3]);
      }
  };

  class CGNSToVtk {
  public:
      template<typename T>
      static void convertTet(T* tet) { }
      template<typename T>
      static void convertPyramid(T* pyramid) { }
      template<typename T>
      static void convertHex(T* hex) { }
      template<typename T>
      static void convertPrism(T* prism) {
          std::swap(prism[0], prism[3]);
          std::swap(prism[1], prism[4]);
          std::swap(prism[2], prism[5]);
      }
      template<typename T>
      static void convertPenta_15(T* cell){
        std::swap(cell[9], cell[12]);
        std::swap(cell[10], cell[13]);
        std::swap(cell[11], cell[14]);
      }
      template<typename T>
      static void convertPenta_18(T* cell){
          std::swap(cell[9], cell[12]);
          std::swap(cell[10], cell[13]);
          std::swap(cell[11], cell[14]);
      }
  };

  class CGNSToFun3D {
  public:
      template<typename T>
      static void convertTet(T* tet) { }
      template<typename T>
      static void convertPyramid(T* pyramid) { }
      template<typename T>
      static void convertPrism(T* prism) {
          std::swap(prism[1], prism[3]);
          auto tmp = prism[2];
          prism[2] = prism[4];
          prism[4] = prism[5];
          prism[5] = tmp;
      }
      template<typename T>
      void static convertHex(T* hex) {
          T tmp = hex[1];
          hex[1] = hex[4];
          hex[4] = hex[3];
          hex[3] = hex[5];
          hex[5] = hex[7];
          hex[7] = hex[6];
          hex[6] = hex[2];
          hex[2] = tmp;
      }
      template<typename T>
      void static convertTetEdges(T* tet_edges){
          T tmp = tet_edges[3];
          tet_edges[3] = tet_edges[1];
          tet_edges[1] = tet_edges[2];
          tet_edges[2] = tmp;
      }

      template<typename T>
      void static convertPyramidEdges(T* pyramid_edges) {
          std::array<T, 8> tmp_pyramid;
          for (int i = 0; i < 8; i++)
              tmp_pyramid[i] = pyramid_edges[i];
          pyramid_edges[0] = tmp_pyramid[4];
          pyramid_edges[1] = tmp_pyramid[5];
          pyramid_edges[2] = tmp_pyramid[6];
          pyramid_edges[3] = tmp_pyramid[7];
          pyramid_edges[4] = tmp_pyramid[0];
          pyramid_edges[5] = tmp_pyramid[1];
          pyramid_edges[6] = tmp_pyramid[2];
          pyramid_edges[7] = tmp_pyramid[3];
      }

      template<typename T>
      void static convertPrismEdges(T*prism_edges) {
          std::array<T, 9> tmp_prism;
          for (int i = 0; i < 9; i++)
              tmp_prism[i] = prism_edges[i];
          prism_edges[0] = tmp_prism[3];
          prism_edges[1] = tmp_prism[0];
          prism_edges[2] = tmp_prism[2];
          prism_edges[3] = tmp_prism[6];
          prism_edges[4] = tmp_prism[8];
          prism_edges[5] = tmp_prism[4];
          prism_edges[6] = tmp_prism[7];
          prism_edges[7] = tmp_prism[1];
          prism_edges[8] = tmp_prism[5];
      }

      template<typename T>
      void static convertHexEdges(T* hex_edges){
          std::array<T,12> tmp_hex;
          for(int i=0;i<12;i++)
              tmp_hex[i] = hex_edges[i];
          hex_edges[0] = tmp_hex[4];
          hex_edges[1] = tmp_hex[0];
          hex_edges[2] = tmp_hex[3];
          hex_edges[3] = tmp_hex[8];
          hex_edges[4] = tmp_hex[11];
          hex_edges[5] = tmp_hex[5];
          hex_edges[6] = tmp_hex[1];
          hex_edges[7] = tmp_hex[9];
          hex_edges[8] = tmp_hex[7];
          hex_edges[9] = tmp_hex[2];
          hex_edges[10] = tmp_hex[10];
          hex_edges[11] = tmp_hex[6];
      }


  };

  class AflrToCGNS {
  public:
      template<typename T>
      static void convertTet(T* tet) { }
      template<typename T>
      static void convertPrism(T* prism) { }
      template<typename T>
      static void convertHex(T* hex) { }
      template<typename T>
      static void convertPyramid(T* pyramid) {
          std::swap(pyramid[2], pyramid[4]);
          std::swap(pyramid[1], pyramid[3]);
      }
  };

  class Fun3DToCGNS {
  public:
      template<typename T>
      static void convertTet(T* tet) { }
      template<typename T>
      static void convertPyramid(T* pyramid) { }
      template<typename T>
      static void convertPrism(T* prism) {
          std::swap(prism[1], prism[3]);
          auto tmp = prism[2];
          prism[2] = prism[5];
          prism[5] = prism[4];
          prism[4] = tmp;
      }

      template<typename T>
      static void convertHex(T* hex) {
          auto tmp = hex[1];
          hex[1] = hex[2];
          hex[2] = hex[6];
          hex[6] = hex[7];
          hex[7] = hex[5];
          hex[5] = hex[3];
          hex[3] = hex[4];
          hex[4] = tmp;
      }
  };

  class VtkToCGNS {
  public:
      template<typename T> static void convertTet(T* tet) { }
      template<typename T> static void convertPyramid(T* pyramid) { }
      template<typename T> static void convertHex(T* hex) { }

      template<typename T> static void convertPrism(T* prism) {
          std::swap(prism[0], prism[3]);
          std::swap(prism[1], prism[4]);
          std::swap(prism[2], prism[5]);
      }
  };
}
