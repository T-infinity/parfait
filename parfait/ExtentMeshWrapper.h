
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
#include "Extent.h"

namespace Parfait{
    class ExtentMeshWrapper{
    public:
        ExtentMeshWrapper() = default;
        ExtentMeshWrapper(const Extent<double>& e):extents({e}){}
        ExtentMeshWrapper(const std::vector<Extent<double>>& vec):extents(vec){}
        int numberOfNodes() {return 8*extents.size();}
        int numberOfCells() {return extents.size();}
        int numberOfNodesInCell(int i) {return 8;}
        Point<double> getNode(int i) {
            auto& e = extents[i/8];
            int vertex = i%8;
            auto p = e.lo;
            if(1 == vertex or 2 == vertex or 5 == vertex or 6 == vertex)
                p[0] += e.getLength_X();
            if(2 == vertex or 3 == vertex or 6 == vertex or 7 == vertex)
                p[1] += e.getLength_Y();
            if(3 < vertex)
                p[2] += e.getLength_Z();
            return p;
        }
		Point<double> getPoint(int i){return getNode(i);}
        std::array<int,8> getNodesInCell(int i){
            int n = i*8;
            return {n,n+1,n+2,n+3,n+4,n+5,n+6,n+7};
        };
        void addExtent(const Extent<double>& e){ extents.push_back(e); }
    private:
        std::vector<Parfait::Extent<double>> extents;
    };
}

