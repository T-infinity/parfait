
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
#include <memory>
#include <MessagePasser/MessagePasser.h>
#include <parfait/ParallelMesh.h>
#include <parfait/NodeCenteredRedistributor.h>


TEST_CASE("Node Centered Redistributor"){
    MessagePasser mp;
    if(mp.NumberOfProcesses() != 2)
        return;

    Parfait::ParallelMeshBuilder meshBuilder(mp);
    std::vector<int> part(3, mp.Rank());
    if(mp.Rank() == 0){
        meshBuilder.data->prisms = {0,1,2,3,4,5};
        meshBuilder.data->nodeOwnerRank = {0,0,0,1,1,1};
        meshBuilder.data->globalNodeIds = {0,1,2,3,4,5};
        meshBuilder.data->triangles = {0,1,2};
        meshBuilder.data->triangleTags = {1};
        meshBuilder.data->nodeComponentIds = {0,0,0};
        meshBuilder.data->xyz = {0,0,0, 1,0,0, 1,1,0, 0,0,1, 1,0,1, 1,1,1};
    }

    if(mp.Rank() == 1){
        meshBuilder.data->prisms = {3,4,5,0,1,2};
        meshBuilder.data->nodeOwnerRank = {1,1,1,0,0,0};
        meshBuilder.data->triangles = {0,1,2};
        meshBuilder.data->triangleTags = {1};
        meshBuilder.data->globalNodeIds = {3,4,5,0,1,2};
        meshBuilder.data->nodeComponentIds = {0,0,0};
        meshBuilder.data->xyz = {0,0,1, 1,0,1, 1,1,1, 0,0,0, 1,0,0, 1,1,0,};
    }

    auto mesh = std::make_shared<Parfait::ParallelMesh>(mp, meshBuilder);
    Parfait::NodeBasedRedistributor reDistributor(mp,mesh, part);
    mesh = reDistributor.redistribute();

    int number_of_owned_nodes = 0;
    for(int d = 0; d < mesh->numberOfNodes(); d++){
        if(not mesh->isGhostNode(d))
            number_of_owned_nodes++;
    }

    auto total = mp.ParallelSum(number_of_owned_nodes);
    REQUIRE(6 == total);

    REQUIRE(mesh->numberOfNodes() == 6);
}
