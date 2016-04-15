#ifndef PARFAIT_CELL_WINDING_CONVERTERS_H
#define PARFAIT_CELL_WINDING_CONVERTERS_H

#include <functional>

namespace Parfait{

  class CGNSToAflr {
  public:
      template <typename T> static void convertTet(T *tet){}
      template <typename T> static void convertPrism(T* prism){}
      template <typename T> static void convertHex(T*hex){}

      template <typename T>
      static void convertPyramid(T* pyramid){
          auto tmp = pyramid[2];
          pyramid[2] = pyramid[4];
          pyramid[4] = pyramid[3];
          pyramid[3] = tmp;
      }
  };

  class CGNSToVtk {
  public:
      template <typename T> static void convertTet(T *tet){}
      template <typename T> static void convertPyramid(T* pyramid){}
      template <typename T> static void convertHex(T*hex){}
      static void convertPrism(int* prism){
          std::swap(prism[0],prism[3]);
          std::swap(prism[1],prism[4]);
          std::swap(prism[2],prism[5]);
      }
  };

  class CGNSToFun3D {
  public:
      template <typename T> static void convertTet(T *tet){}
      template <typename T> static void convertPyramid(T* pyramid){}
      template <typename T>
      static void convertPrism(T* prism){
          std::swap(prism[1],prism[3]);
          auto tmp = prism[2];
          prism[2] = prism[4];
          prism[4] = prism[5];
          prism[5] = tmp;
      }
      template <typename T>
      void static convertHex(T* hex){
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
      template <typename T> static void convertTet(T *tet){}
      template <typename T> static void convertPrism(T* prism){}
      template <typename T> static void convertHex(T*hex){}
      template <typename T>
      static void convertPyramid(T *pyramid) {
          T tmp = pyramid[2];
          pyramid[2] = pyramid[3];
          pyramid[3] = pyramid[4];
          pyramid[4] = tmp;
      }
  };

  class Fun3DToCGNS {
  public:
      template <typename T> static void convertTet(T *tet){}
      template <typename T> static void convertPyramid(T* pyramid){}
      template <typename T>
      static void convertPrism(T* prism){
          std::swap(prism[1],prism[3]);
          auto tmp = prism[2];
          prism[2] = prism[5];
          prism[5] = prism[4];
          prism[4] = tmp;
      }

      template <typename T>
      static void convertHex(T* hex){
          auto tmp = hex[1];
          hex[1] = hex[2];
          hex[2] = hex[6];
          hex[6] = hex[7];
          hex[7] = hex[5];
          hex[5] = hex[3];
          hex[3] = hex[4];
          hex[4] =tmp;
      }
  };

  class VtkToCGNS {
  public:
      template <typename T> static void convertTet(T *tet){}
      template <typename T> static void convertPyramid(T* pyramid){}
      template <typename T> static void convertHex(T* hex){}

      template <typename T>
      static void convertPrism(T* prism){
          std::swap(prism[0],prism[3]);
          std::swap(prism[1],prism[4]);
          std::swap(prism[2],prism[5]);
      }
  };
}

#endif
