
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
#include <parfait/ParallelMeshReader.h>
#include <parfait/PreProcessor.h>
#include <catch.hpp>

using namespace Parfait;

TEST_CASE("Read in grid based on json file (on one proc)") {
    MessagePasser mp;
    std::string input_json = SIX_CELL_TEST_JSON;
    ConfigurationReader reader(input_json);
    Configuration config = reader.createConfiguration();

    PreProcessor preProcessor(mp, config);
    auto mesh = preProcessor.createMesh();

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

    if(mp.NumberOfProcesses() == 2){
        REQUIRE(mesh->numberOfNodes() == 14);
    }

    BoundaryConditionMap boundaryConditionMap = config.getMapbcObject(0);
}

TEST_CASE("Add Owning Rank test"){
    MessagePasser mp;
    std::string input_json = SIX_CELL_TEST_JSON;
    ConfigurationReader reader(input_json);
    Configuration config = reader.createConfiguration();

    PreProcessor preProcessor(mp, config);
    auto mesh = preProcessor.createMesh();

    if(mp.NumberOfProcesses() == 2){
        int other  = (mp.Rank() == 0)?(1):(0);
        for(int i = 0; i < 7; i++){
            REQUIRE(mp.Rank() == mesh->owningRankOfNode(i));
        }
        for(int i = 7; i < 14; i++){
            REQUIRE(other == mesh->owningRankOfNode(i));
        }
    }
}
