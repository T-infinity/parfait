
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
#include <parfait/NodeToCell.h>

using namespace Parfait;

TEST_CASE("Build Node To Cell from a MeshTemplate"){
	CartBlock block({0,0,0},{1,1,1},2,2,2);
	auto n2c = buildNodeToCell(block);

	REQUIRE(block.numberOfNodes() == n2c.size());
	REQUIRE(1 == n2c[0].size());
	REQUIRE(0 == n2c[0][0]);
	REQUIRE(2 == n2c[1].size());
}
