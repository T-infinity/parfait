
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
