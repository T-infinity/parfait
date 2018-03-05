
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
#include <vector>
#include <parfait/ExtentBuilder.h>
#include <parfait/CartMesh.h>

using std::vector;
using namespace Parfait;

class PointCloud {
public:
	int numberOfNodes() const {return nodes.size()/3;}
	void getNode(int id,double coords[3]) const {
		coords[0] = nodes[3*id+0];
		coords[1] = nodes[3*id+1];
		coords[2] = nodes[3*id+2];
	}
	Point<double> getNode(int id) const{
		Point<double> p{nodes[3*id],nodes[3*id+1],nodes[3*id+2]};
		return p;
	}
	vector<double> nodes;
};


TEST_CASE("ExtentBuilderTests,Exists") {
	PointCloud mesh;
	mesh.nodes = {0.0, 0.0, 0.0,
				  1.0, 0.0, 0.0,
				  0.5, 1.0, 0.3,
				  0.7, 0.1, 1.0};

	Extent<double> extent = ExtentBuilder::build(mesh);
	REQUIRE(0.0 == extent.lo[0]);
	REQUIRE(0.0 == extent.lo[1]);
	REQUIRE(0.0 == extent.lo[2]);
	REQUIRE(1.0 == extent.hi[0]);
	REQUIRE(1.0 == extent.hi[1]);
	REQUIRE(1.0 == extent.hi[2]);
}

TEST_CASE("ExtentBuilderTests,expandExtentWithAnother"){
	Extent<int> e1{{0,0,0},{1,1,1}};
	Extent<int> e2{{1,-1,2},{0,1,3}};
	ExtentBuilder::expandExtentWithAnother(e1,e2);
	REQUIRE(0 == e1.lo[0]);
	REQUIRE(-1 == e1.lo[1]);
	REQUIRE(0 == e1.lo[2]);
	REQUIRE(1 == e1.hi[0]);
	REQUIRE(1 == e1.hi[1]);
	REQUIRE(3 == e1.hi[2]);
}

TEST_CASE("ExtentBuilderTests,addPointToExtent") {
	std::vector<Point<double>> points;
	points.push_back({0.0, 0.0, 0.});
	points.push_back({1.0, 0.0, 0.0});
	points.push_back({0.5, 1.0, 0.3});
	points.push_back({0.7, 0.1, 1.0});
	Extent<double> e({.5,.5,.5},{.5,.5,.5});
	for(int i=0;i<4;i++)
		ExtentBuilder::addPointToExtent(e,points[i]);
	REQUIRE(0.0 == e.lo[0]);
	REQUIRE(0.0 == e.lo[1]);
	REQUIRE(0.0 == e.lo[2]);
	REQUIRE(1.0 == e.hi[0]);
	REQUIRE(1.0 == e.hi[1]);
	REQUIRE(1.0 == e.hi[2]);
};


TEST_CASE("ExtentBuilderTests,createExtentForMeshCell"){
	Parfait::CartMesh one_cell_cart_mesh({0,0,0},{2,2,2},1,1,1);
	Extent<double> e = ExtentBuilder::buildExtentForCellInMesh(one_cell_cart_mesh,0);
	REQUIRE(0.0 == e.lo[0]);
	REQUIRE(0.0 == e.lo[1]);
	REQUIRE(0.0 == e.lo[2]);
	REQUIRE(2.0 == e.hi[0]);
	REQUIRE(2.0 == e.hi[1]);
	REQUIRE(2.0 == e.hi[2]);
}

TEST_CASE("ExtentBuilderTests,createExtentForBoundaryFaceInMesh"){

	class MockFaceMesh{
	public:
		std::vector<int> getNodesInBoundaryFace(int i) const{
			return {0,1,7};
		}
		Parfait::Point<double> getNode(int i) const {
			return Parfait::Point<double>(i,i,i);
		}
	}mockFaceMesh;

	Extent<double> e = ExtentBuilder::buildExtentForBoundaryFaceInMesh(mockFaceMesh,0);
	REQUIRE(0.0 == e.lo[0]);
	REQUIRE(0.0 == e.lo[1]);
	REQUIRE(0.0 == e.lo[2]);
	REQUIRE(7.0 == e.hi[0]);
	REQUIRE(7.0 == e.hi[1]);
	REQUIRE(7.0 == e.hi[2]);
}
