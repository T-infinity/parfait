
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
#include <string>
#include <catch.hpp>
#include <parfait/ParallelMeshReader.h>


TEST_CASE("Read in the grid and check it if run on one proc") {
    MessagePasser mp;
    auto mesh = Parfait::ParallelMeshReader::readDistributedGrid(
            mp, {"../../grids/6cell.lb8.ugrid"}, {false});
    if(mp.NumberOfProcesses() == 1){
        REQUIRE(12 == mesh->numberOfTriangles());
        REQUIRE(6 == mesh->numberOfQuads());
        REQUIRE(0 == mesh->numberOfTets());
        REQUIRE(0 == mesh->numberOfPyramids());
        REQUIRE(6 == mesh->numberOfPrisms());
        REQUIRE(0 == mesh->numberOfHexes());
        for(int i = 0; i < 14; i++){
            REQUIRE(i == mesh->getGlobalNodeId(i));
        }
        for(int i = 0; i < 14; i++){
            REQUIRE(not mesh->isGhostNode(i));
        }
    }
}

TEST_CASE("Total number of nodes and grids is correct globally"){
    MessagePasser mp;
    auto reader = Parfait::ParallelMeshReader(mp, {"../../grids/6cell.lb8.ugrid"}, {false});
    REQUIRE(14 == reader.totalNumberOfNodes());
    REQUIRE(1 == reader.numberOfGrids());
}
