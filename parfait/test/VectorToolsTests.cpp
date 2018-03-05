
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
#include <parfait/VectorTools.h>

using std::vector;

TEST_CASE("VectorToolsTests,TestWithIntegers") {
	vector<int> vec;
	Parfait::insertUnique(vec,0);
	REQUIRE(1 == (int)vec.size());
	REQUIRE(0 == vec[0]);

	Parfait::insertUnique(vec,0);
	REQUIRE(1 == (int)vec.size());

	Parfait::insertUnique(vec,1);
	REQUIRE(2 == (int)vec.size());

	for(int i=9;i>0;i--)
		Parfait::insertUnique(vec,i);
	REQUIRE(10 == (int)vec.size());
	for(unsigned int i=0;i<vec.size()-1;i++)
		CHECK(vec[i] < vec[i+1]);
	for(int i=5;i<12;i++)
		Parfait::insertUnique(vec,i);
	REQUIRE(12 == (int)vec.size());
}

