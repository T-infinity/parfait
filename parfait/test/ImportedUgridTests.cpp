
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
#include <vector>
#include <catch.hpp>
#include <parfait/ImportedUgrid.h>

using std::vector;
using namespace Parfait;

template <class MeshType> void constFunction(const MeshType &mesh);

namespace ImportedUgridTests{
	vector<double> nodes;
	vector<int> triangles;
	vector<int> quads;
	vector<int> tets;
	vector<int> pyramids;
	vector<int> prisms;
	vector<int> hexs;
    vector<int> triangleTags;
    vector<int> quadTags;
    vector<int> triangleBoundaryConditions;
    vector<int> quadBoundaryConditions;
	void setup()
	{
		nodes = {0.0,0.0,0.0,
				 1.0,0.0,0.0,
				 1.0,1.0,0.0,
		         0.0,0.0,1.0};
		triangles = {0,1,2,
					 0,1,3,
					 0,3,2,
					 2,3,1};
        triangleTags = {1,2,3,4};
		tets = {0,1,2,3};
		triangleBoundaryConditions = {5050,5050,5050,5050};
	}
};

TEST_CASE("ImportedUgridTests,Exists") {
	using namespace ImportedUgridTests;
	setup();
	ImportedUgrid ugrid(nodes,triangles,quads,
			tets,pyramids,prisms,hexs, triangleTags, quadTags);

	REQUIRE(4 == ugrid.numberOfNodes());
	REQUIRE(1 == ugrid.numberOfCells());
	REQUIRE(4 == ugrid.numberOfBoundaryFaces());
	REQUIRE(4 == ugrid.numberOfFacesInCell(0));
	REQUIRE(3 == ugrid.numberOfNodesInCellFace(0,0));
}