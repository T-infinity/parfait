
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
#include <array>
#include <catch.hpp>
#include <parfait/Point.h>
#include <parfait/Metrics.h>
#include <parfait/CGNSElements.h>
#include <parfait/DualMetrics.h>

TEST_CASE("Triangle Area"){

    std::array<Parfait::Point<double>, 3> tri;
    tri[0] = {0,0,0};
    tri[1] = {1,0,0};
    tri[2] = {0,1,0};

    auto area = Parfait::Metrics::computeTriangleArea(tri[0], tri[2], tri[1]);
    REQUIRE(area.magnitude() == Approx(0.5));
    REQUIRE(area[2] == Approx(-0.5));
}

TEST_CASE("Tet Edge Centers"){
    std::array<Parfait::Point<double>, 4> tet;
    tet[0] = {0,0,0};
    tet[1] = {1,0,0};
    tet[2] = {0,1,0};
    tet[3] = {0,0,1};

    auto edge_centers = Parfait::CGNS::Tet::computeEdgeCenters(tet);
    REQUIRE(edge_centers.size() == 6);

    REQUIRE(edge_centers[0][0] == Approx(0.5));
    REQUIRE(edge_centers[0][1] == Approx(0.0));
    REQUIRE(edge_centers[0][2] == Approx(0.0));

    REQUIRE(edge_centers[1][0] == Approx(0.5));
    REQUIRE(edge_centers[1][1] == Approx(0.5));
    REQUIRE(edge_centers[1][2] == Approx(0.0));

    REQUIRE(edge_centers[2][0] == Approx(0.0));
    REQUIRE(edge_centers[2][1] == Approx(0.5));
    REQUIRE(edge_centers[2][2] == Approx(0.0));

    REQUIRE(edge_centers[3][0] == Approx(0.0));
    REQUIRE(edge_centers[3][1] == Approx(0.0));
    REQUIRE(edge_centers[3][2] == Approx(0.5));

    REQUIRE(edge_centers[4][0] == Approx(0.5));
    REQUIRE(edge_centers[4][1] == Approx(0.0));
    REQUIRE(edge_centers[4][2] == Approx(0.5));

    REQUIRE(edge_centers[5][0] == Approx(0.0));
    REQUIRE(edge_centers[5][1] == Approx(0.5));
    REQUIRE(edge_centers[5][2] == Approx(0.5));
}

TEST_CASE("Tet Face Centers"){
    std::array<Parfait::Point<double>, 4> tet;
    tet[0] = {0,0,0};
    tet[1] = {1,0,0};
    tet[2] = {0,1,0};
    tet[3] = {0,0,1};

    auto face_centers = Parfait::CGNS::Tet::computeFaceCenters(tet);
    REQUIRE(face_centers.size() == 4);

    double third = 1.0/3.0;
    REQUIRE(face_centers[0][0] == Approx(third));
    REQUIRE(face_centers[0][1] == Approx(third));
    REQUIRE(face_centers[0][2] == Approx(0.0));

    REQUIRE(face_centers[1][0] == Approx(third));
    REQUIRE(face_centers[1][1] == Approx(0));
    REQUIRE(face_centers[1][2] == Approx(third));

    REQUIRE(face_centers[2][0] == Approx(third));
    REQUIRE(face_centers[2][1] == Approx(third));
    REQUIRE(face_centers[2][2] == Approx(third));

    REQUIRE(face_centers[3][0] == Approx(0));
    REQUIRE(face_centers[3][1] == Approx(third));
    REQUIRE(face_centers[3][2] == Approx(third));
}

TEST_CASE("Tet Edge To Node"){
    auto& edge_to_node = Parfait::CGNS::Tet::edge_to_node;
    REQUIRE((edge_to_node[0] == std::array<int,2>{0, 1}));
    REQUIRE((edge_to_node[1] == std::array<int,2>{1, 2}));
    REQUIRE((edge_to_node[2] == std::array<int,2>{2, 0}));
    REQUIRE((edge_to_node[3] == std::array<int,2>{0, 3}));
    REQUIRE((edge_to_node[4] == std::array<int,2>{1, 3}));
    REQUIRE((edge_to_node[5] == std::array<int,2>{2, 3}));
}

TEST_CASE("Tet Edge To Face"){
    auto& edge_to_face = Parfait::CGNS::Tet::edge_to_face;
    REQUIRE((edge_to_face[0] == std::array<int,2>{0, 1}));
    REQUIRE((edge_to_face[1] == std::array<int,2>{0, 2}));
    REQUIRE((edge_to_face[2] == std::array<int,2>{0, 3}));
    REQUIRE((edge_to_face[3] == std::array<int,2>{1, 3}));
    REQUIRE((edge_to_face[4] == std::array<int,2>{2, 1}));
    REQUIRE((edge_to_face[5] == std::array<int,2>{3, 2}));
}

TEST_CASE("Tet area calculations edge 0") {
    std::array<Parfait::Point<double>, 4> tet;
    tet[0] = {0, 0, 0};
    tet[1] = {1, 0, 0};
    tet[2] = {0.5, 0.5, 0};
    tet[3] = {0.5, 0.5, 1};

    auto areas = calculateTetAreas(tet);
    REQUIRE(6 == areas.size());
    auto n = areas[0];
    n.normalize();
    REQUIRE(n[0] == Approx(1));
    REQUIRE(n[1] == Approx(0));
    REQUIRE(n[2] == Approx(0));
}

TEST_CASE("Tet area calculations edge 1") {
    std::array<Parfait::Point<double>, 4> tet;
    tet[0] = {0.5, 0.5, 0};
    tet[1] = {0, 0, 0};
    tet[2] = {1, 0, 0};
    tet[3] = {0.5, 0.5, 1};

    auto areas = calculateTetAreas(tet);
    REQUIRE(6 == areas.size());
    auto n = areas[1];
    n.normalize();
    REQUIRE(n[0] == Approx(1));
    REQUIRE(n[1] == Approx(0));
    REQUIRE(n[2] == Approx(0));
}

TEST_CASE("Tet area calculations edge 2") {
    std::array<Parfait::Point<double>, 4> tet;
    tet[0] = {1, 0, 0};
    tet[1] = {0.5, 0.5, 0};
    tet[2] = {0, 0, 0};
    tet[3] = {0.5, 0.5, 1};

    auto areas = calculateTetAreas(tet);
    REQUIRE(6 == areas.size());
    auto n = areas[2];
    n.normalize();
    REQUIRE(n[0] == Approx(1));
    REQUIRE(n[1] == Approx(0));
    REQUIRE(n[2] == Approx(0));
}

TEST_CASE("Tet area calculations edge 3") {
    std::array<Parfait::Point<double>, 4> tet;
    tet[0] = {0, 0, 0};
    tet[1] = {1, 0, 0.5};
    tet[2] = {0, 1, 0.5};
    tet[3] = {0, 0, 1};

    auto areas = calculateTetAreas(tet);
    REQUIRE(6 == areas.size());
    auto n = areas[3];
    n.normalize();
    REQUIRE(n[0] == Approx(0));
    REQUIRE(n[1] == Approx(0));
    REQUIRE(n[2] == Approx(1));
}
