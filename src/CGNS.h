#ifndef PARFAIT_CGNS_H
#define PARFAIT_CGNS_H
namespace Parfait {
  namespace CGNS {
    namespace Edges {
      namespace Tet {
        constexpr int numberOfEdges() {
            return 6;
        }
        inline std::array<int,2> getEdge(int edge){
            switch (edge) {
                case 0: return {0,1};
                case 1: return {1,2};
                case 2: return {2,0};
                case 3: return {0,3};
                case 4: return {1,3};
                case 5: return {2,3};
            }
          throw std::logic_error("invalid edge");
        };
        template <class Tet>
        std::array<int, 2> getEdge(const Tet& tet, int edge) {
            auto e = getEdge(edge);
            return {tet[e[0]], tet[e[1]]};
        }
      }
      namespace Pyramid {
        constexpr int numberOfEdges() {
            return 8;
        }
        inline std::array<int,2> getEdge(int edge){
            switch (edge) {
                case 0: return {0,1};
                case 1: return {1,2};
                case 2: return {2,3};
                case 3: return {3,0};
                case 4: return {0,4};
                case 5: return {1,4};
                case 6: return {2,4};
                case 7: return {3,4};
            }
          throw std::logic_error("invalid edge");
        }
        template <class Pyramid>
        std::array<int, 2> getEdge(const Pyramid& pyramid, int edge) {
            auto e = getEdge(edge);
            return {pyramid[e[0]], pyramid[e[1]]};
        }
      }
      namespace Prism {
        constexpr int numberOfEdges() {
            return 9;
        }
        inline std::array<int,2> getEdge(int edge){
            switch (edge) {
                case 0: return {0,1};
                case 1: return {1,2};
                case 2: return {2,0};
                case 3: return {0,3};
                case 4: return {1,4};
                case 5: return {2,5};
                case 6: return {3,4};
                case 7: return {4,5};
                case 8: return {5,3};
            }
          throw std::logic_error("invalid edge");
        }
        template <class Prism>
        std::array<int, 2> getEdge(const Prism& prism, int edge) {
            auto e = getEdge(edge);
            return {prism[e[0]], prism[e[1]]};
        }
      }
      namespace Hex {
        constexpr int numberOfEdges() {
            return 12;
        }
        inline std::array<int,2> getEdge(int edge){
            switch (edge) {
                case 0: return {0,1};
                case 1: return {1,2};
                case 2: return {2,3};
                case 3: return {3,0};
                case 4: return {0,4};
                case 5: return {1,5};
                case 6: return {2,7};
                case 7: return {3,7};
                case 8: return {4,5};
                case 9: return {5,6};
                case 10: return {6,7};
                case 11: return {7,4};
            }
          throw std::logic_error("invalid edge");

        }
        template <class Hex>
        std::array<int, 2> getEdge(const Hex& hex, int edge) {
            auto e = getEdge(edge);
            return {hex[e[0]], hex[e[1]]};
        }
      }
    }
  }
}
#endif 
