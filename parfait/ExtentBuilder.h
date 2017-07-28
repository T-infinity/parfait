#pragma once

#include "Extent.h"
#include <limits>

namespace Parfait {
    namespace ExtentBuilder {

        template<typename T>
        Extent<T> createEmptyBuildableExtent(const Extent<T>&& e){
            T lo = std::numeric_limits<T>::lowest();
            T hi = std::numeric_limits<T>::max();
            return Extent<T> {{hi,hi,hi},{lo,lo,lo}};
        }

        template<typename T>
        void expandExtentWithAnother(Extent<T>& e1,const Extent<T>& e2){
            for(int i=0;i<3;i++){
                e1.lo[i] = std::min(e1.lo[i],e2.lo[i]);
                e1.hi[i] = std::max(e1.hi[i],e2.hi[i]);
            }
        }

        template <typename T>
        Extent<T> getBoundingBox(const std::vector<Extent<T>>& boxes){
            if(boxes.empty()) return Extent<T>();
            Extent<T> out{boxes.front()};
            for (const auto &ext : boxes)
                expandExtentWithAnother(out,ext);
            return out;
        }

        template<typename T>
        void addPointToExtent(Extent<T>& e, const Point<T>& p) {
            for(int i=0;i<3;i++) {
                e.lo[i] = std::min(e.lo[i], p[i]);
                e.hi[i] = std::max(e.hi[i], p[i]);
            }
        }

        template<typename ObjectWithPoints>
        auto build(const ObjectWithPoints& obj){
            auto p = obj.getNode(0);
            auto x = p[0]; auto y = p[1]; auto z = p[2];
            Extent<decltype(x)> e{{x,y,z},{x,y,z}};
            for(int i=1;i< obj.numberOfNodes();i++)
                addPointToExtent(e, obj.getNode(i));
            return e;
        }

        template<typename MeshType>
        Extent<double> buildExtentForCellInMesh(const MeshType& mesh,int cell_id){
            auto e = createEmptyBuildableExtent(Extent<double>());
            for(int node_id:mesh.getNodesInCell(cell_id)){
                auto xyz = mesh.getNode(node_id);
                addPointToExtent(e,Point<double>(xyz[0],xyz[1],xyz[2]));
            }
            return e;
        }

        template<typename MeshType>
        Extent<double> buildExtentForBoundaryFaceInMesh(const MeshType& mesh,int face_id){
            Extent<double> e{{1e20,1e20,1e20},{-1e20,-1e20,-1e20}};
            auto face = mesh.getNodesInBoundaryFace(face_id);
            for(int node_id:face){
                auto xyz = mesh.getNode(node_id);
                addPointToExtent(e,Point<double>(xyz[0],xyz[1],xyz[2]));
            }
            return e;
        }
    }
}