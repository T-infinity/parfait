
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
#include <parfait/LeastSquaresGradient.h>

TEST_CASE("Gram-Schmidt weight calculation, unit directions, check diagon"){
    std::vector<Parfait::Point<double>> points = {{1,0,0},{0,1,0},{0,0,1}};
    Parfait::Point<double> center = {0,0,0};
    auto gs = Parfait::calcGramSchmidtWeights(points, center);
    REQUIRE(gs.r11 == Approx(1));
    REQUIRE(gs.r12 == Approx(0));
    REQUIRE(gs.r13 == Approx(0));
    REQUIRE(gs.r22 == Approx(1));
    REQUIRE(gs.r23 == Approx(0));
    REQUIRE(gs.r33 == Approx(1));
}

TEST_CASE("Gram-Schmidt weight calculation, x and z off diag"){
    std::vector<Parfait::Point<double>> points = {{0,0,0},{1,1,0},{0,0,1}};
    Parfait::Point<double> center = {1,0,0};
    auto gs = Parfait::calcGramSchmidtWeights(points, center);
    REQUIRE(gs.r11 == Approx( 1.41421));
    REQUIRE(gs.r12 == Approx(0));
    REQUIRE(gs.r13 == Approx(-0.70711));
    REQUIRE(gs.r22 == Approx(1));
    REQUIRE(gs.r23 == Approx(0));
    REQUIRE(gs.r33 == Approx(0.70711));
}

TEST_CASE("Least Squares gradient calculation"){
    std::vector<Parfait::Point<double>> points = {{1,0,0},{0,1,0},{0,0,1}};
    Parfait::Point<double> center = {0,0,0};
    std::vector<double> u = {5, 1, 3}; // 5x + y + 3z

    auto gs= Parfait::calcGramSchmidtWeights(points, center);
    REQUIRE(gs.r11 == Approx(1));
    REQUIRE(gs.r12 == Approx(0));
    REQUIRE(gs.r13 == Approx(0));
    REQUIRE(gs.r22 == Approx(1));
    REQUIRE(gs.r23 == Approx(0));
    REQUIRE(gs.r33 == Approx(1));

    double u0 = 0;
    auto grad = Parfait::calcLeastSquaresGradient(points, center, gs, u, u0);
    REQUIRE(grad[0] == Approx(5));
    REQUIRE(grad[1] == Approx(1));
    REQUIRE(grad[2] == Approx(3));
}