
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
#include <parfait/StringTools.h>


TEST_CASE("String split by delimiter"){
    std::string s = "This is a sentence.";
    auto l = Parfait::StringTools::split(s, " ");
    REQUIRE(l.size() == 4);
    REQUIRE("This" == l[0]);
    REQUIRE("is" == l[1]);
    REQUIRE("a" == l[2]);
    REQUIRE("sentence." == l[3]);
}

TEST_CASE("String split by string delimiter"){
    std::string s = "(>'-')> <('-'<) ^(' - ')^ <('-'<) (>'-')>";
    auto l = Parfait::StringTools::split(s, "'-'");
    REQUIRE(l.size() == 5);

}
