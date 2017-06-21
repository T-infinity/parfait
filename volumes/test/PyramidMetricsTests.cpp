#include <catch.hpp>
#include <CGNS.h>

TEST_CASE("Pyramid Edge to node"){

    auto& edge_to_node = Parfait::CGNS::Pyramid::edge_to_node;
    REQUIRE((edge_to_node[0]  == std::array<int,2>{0, 1}));
    REQUIRE((edge_to_node[1]  == std::array<int,2>{1, 2}));
    REQUIRE((edge_to_node[2]  == std::array<int,2>{2, 3}));
    REQUIRE((edge_to_node[3]  == std::array<int,2>{3, 0}));
    REQUIRE((edge_to_node[4]  == std::array<int,2>{0, 4}));
    REQUIRE((edge_to_node[5]  == std::array<int,2>{1, 4}));
    REQUIRE((edge_to_node[6]  == std::array<int,2>{2, 4}));
    REQUIRE((edge_to_node[7]  == std::array<int,2>{3, 4}));
}

TEST_CASE("Pyramid Edge to face"){
    auto& edge_to_face = Parfait::CGNS::Pyramid::edge_to_face;
    REQUIRE((edge_to_face[0]  == std::array<int,2>{0, 1}));
    REQUIRE((edge_to_face[1]  == std::array<int,2>{0, 2}));
    REQUIRE((edge_to_face[2]  == std::array<int,2>{0, 3}));
    REQUIRE((edge_to_face[3]  == std::array<int,2>{0, 4}));
    REQUIRE((edge_to_face[4]  == std::array<int,2>{1, 4}));
    REQUIRE((edge_to_face[5]  == std::array<int,2>{2, 1}));
    REQUIRE((edge_to_face[6]  == std::array<int,2>{3, 2}));
    REQUIRE((edge_to_face[7]  == std::array<int,2>{4, 3}));
}

TEST_CASE("Pyramid Edge Centers"){
    std::array<Parfait::Point<double>, 5> pyramid;
    pyramid[0] = {0,0,0};
    pyramid[1] = {1,0,0};
    pyramid[2] = {1,1,0};
    pyramid[3] = {0,1,0};
    pyramid[4] = {0,0,1};

    auto edge_centers = Parfait::CGNS::Pyramid::computeEdgeCenters(pyramid);
    REQUIRE(edge_centers.size() == 8);

    REQUIRE(edge_centers[0][0] == Approx(0.5));
    REQUIRE(edge_centers[0][1] == Approx(0.0));
    REQUIRE(edge_centers[0][2] == Approx(0.0));

    REQUIRE(edge_centers[1][0] == Approx(1.0));
    REQUIRE(edge_centers[1][1] == Approx(0.5));
    REQUIRE(edge_centers[1][2] == Approx(0.0));

    REQUIRE(edge_centers[2][0] == Approx(0.5));
    REQUIRE(edge_centers[2][1] == Approx(1.0));
    REQUIRE(edge_centers[2][2] == Approx(0.0));

    REQUIRE(edge_centers[3][0] == Approx(0.0));
    REQUIRE(edge_centers[3][1] == Approx(0.5));
    REQUIRE(edge_centers[3][2] == Approx(0.0));

    REQUIRE(edge_centers[4][0] == Approx(0.0));
    REQUIRE(edge_centers[4][1] == Approx(0.0));
    REQUIRE(edge_centers[4][2] == Approx(0.5));

    REQUIRE(edge_centers[5][0] == Approx(0.5));
    REQUIRE(edge_centers[5][1] == Approx(0.0));
    REQUIRE(edge_centers[5][2] == Approx(0.5));

    REQUIRE(edge_centers[6][0] == Approx(0.5));
    REQUIRE(edge_centers[6][1] == Approx(0.5));
    REQUIRE(edge_centers[6][2] == Approx(0.5));

    REQUIRE(edge_centers[7][0] == Approx(0.0));
    REQUIRE(edge_centers[7][1] == Approx(0.5));
    REQUIRE(edge_centers[7][2] == Approx(0.5));
}

TEST_CASE("Pyramid Face Centers"){
    std::array<Parfait::Point<double>, 5> pyramid;
    pyramid[0] = {0,0,0};
    pyramid[1] = {1,0,0};
    pyramid[2] = {1,1,0};
    pyramid[3] = {0,1,0};
    pyramid[4] = {0,0,1};

    auto face_centers = Parfait::CGNS::Pyramid::computeFaceCenters(pyramid);
    double third = 1.0/3.0;
    REQUIRE(face_centers.size() == 5);
    REQUIRE(face_centers[0][0] == Approx(0.5));
    REQUIRE(face_centers[0][1] == Approx(0.5));
    REQUIRE(face_centers[0][2] == Approx(0.0));

    REQUIRE(face_centers[1][0] == Approx(third));
    REQUIRE(face_centers[1][1] == Approx(0.0));
    REQUIRE(face_centers[1][2] == Approx(third));

    REQUIRE(face_centers[2][0] == Approx(2*third));
    REQUIRE(face_centers[2][1] == Approx(third));
    REQUIRE(face_centers[2][2] == Approx(third));

    REQUIRE(face_centers[3][0] == Approx(third));
    REQUIRE(face_centers[3][1] == Approx(2*third));
    REQUIRE(face_centers[3][2] == Approx(third));

    REQUIRE(face_centers[4][0] == Approx(0));
    REQUIRE(face_centers[4][1] == Approx(third));
    REQUIRE(face_centers[4][2] == Approx(third));
}