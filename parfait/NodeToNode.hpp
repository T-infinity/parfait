
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
#include "VectorTools.h"
#include "CGNSElements.h"
#include "EdgeBuilder.h"
template<typename MeshType>
std::vector <std::vector<int>> Parfait::NodeToNodeBuilder<MeshType>::buildNodeToNodeConnectivity() {
    std::vector<int> cell;
    for(int i=0;i<mesh.numberOfCells();i++){
        int n = mesh.numberOfNodesInCell(i);
        cell.resize(n);
        mesh.getNodesInCell(i,cell.data());
        switch(n){
            case 4:
                addCell(cell,CGNS::Tet::edge_to_node);
                break;
            case 5:
                addCell(cell,CGNS::Pyramid::edge_to_node);
                break;
            case 6:
                addCell(cell,CGNS::Prism::edge_to_node);
                break;
            case 8:
                addCell(cell,CGNS::Hex::edge_to_node);
                break;
            default:
                throw std::logic_error("NodeToNodeBuilder: only linear volume elements supported");
        }
    }

    std::vector<std::vector<int>> n2n;
    for (auto& row:node_to_node)
        n2n.push_back(std::vector<int>(row.begin(), row.end()));
    return n2n;
}

