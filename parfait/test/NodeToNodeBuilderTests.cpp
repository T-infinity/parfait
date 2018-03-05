
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
#include <catch.hpp>
#include <parfait/CartBlock.h>
#include <parfait/NodeToNode.h>

using namespace Parfait;

class n2nMockMesh{
public:
    int numberOfNodes() {return 4;}
    int numberOfCells() {return 1;}
    int numberOfNodesInCell(int i) {return 4;}
    void getNodesInCell(int i,int* cell) {
        cell[0] = 0;
        cell[1] = 1;
        cell[2] = 2;
        cell[3] = 3;
    }
};

TEST_CASE("Build Node To Node from a MeshTemplate"){
    n2nMockMesh mesh;
    NodeToNodeBuilder<decltype(mesh)> nodeBuilder(mesh);
    auto n2n = nodeBuilder.buildNodeToNodeConnectivity();

    REQUIRE(mesh.numberOfNodes() == n2n.size());
    REQUIRE(3 == n2n[0].size());
    REQUIRE(1 == n2n[0][0]);
    REQUIRE(2 == n2n[0][1]);
    REQUIRE(3 == n2n[0][2]);
}
