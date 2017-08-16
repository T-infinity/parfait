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
