
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

#include "Extent.h"
#include <limits>

namespace Parfait {
    namespace ExtentBuilder {

        template<typename T>
        Extent<T> createEmptyBuildableExtent(){
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

        template<typename Container>
        auto build(const Container& points){
            auto e = createEmptyBuildableExtent<double>();
            for(auto& p:points)
                addPointToExtent(e,p);
            return e;
        }

        template<typename MeshType>
        Extent<double> buildExtentForCellInMesh(const MeshType& mesh,int cell_id){
            auto e = createEmptyBuildableExtent<double>();
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