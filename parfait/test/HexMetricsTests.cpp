#include <catch.hpp>
#include <parfait/CGNSElements.h>
#include <parfait/DualMetrics.h>

TEST_CASE("Hex Edge to node") {
    
    auto& edge_to_node = Parfait::CGNS::Hex::edge_to_node;
    REQUIRE((edge_to_node[0]  == std::array<int,2>{0, 1}));
    REQUIRE((edge_to_node[1]  == std::array<int,2>{1, 2}));
    REQUIRE((edge_to_node[2]  == std::array<int,2>{2, 3}));
    REQUIRE((edge_to_node[3]  == std::array<int,2>{3, 0}));
    REQUIRE((edge_to_node[4]  == std::array<int,2>{0, 4}));
    REQUIRE((edge_to_node[5]  == std::array<int,2>{1, 5}));
    REQUIRE((edge_to_node[6]  == std::array<int,2>{2, 6}));
    REQUIRE((edge_to_node[7]  == std::array<int,2>{3, 7}));
    REQUIRE((edge_to_node[8]  == std::array<int,2>{4, 5}));
    REQUIRE((edge_to_node[9]  == std::array<int,2>{5, 6}));
    REQUIRE((edge_to_node[10] == std::array<int,2>{6, 7}));
    REQUIRE((edge_to_node[11] == std::array<int,2>{7, 4}));
}

TEST_CASE("Hex Edge to face"){
    auto& edge_to_face = Parfait::CGNS::Hex::edge_to_face;
    REQUIRE((edge_to_face[0]  == std::array<int,2>{0, 1}));
    REQUIRE((edge_to_face[1]  == std::array<int,2>{0, 2}));
    REQUIRE((edge_to_face[2]  == std::array<int,2>{0, 3}));
    REQUIRE((edge_to_face[3]  == std::array<int,2>{0, 4}));
    REQUIRE((edge_to_face[4]  == std::array<int,2>{1, 4}));
    REQUIRE((edge_to_face[5]  == std::array<int,2>{2, 1}));
    REQUIRE((edge_to_face[6]  == std::array<int,2>{3, 2}));
    REQUIRE((edge_to_face[7]  == std::array<int,2>{4, 3}));
    REQUIRE((edge_to_face[8]  == std::array<int,2>{1, 5}));
    REQUIRE((edge_to_face[9]  == std::array<int,2>{2, 5}));
    REQUIRE((edge_to_face[10] == std::array<int,2>{3, 5}));
    REQUIRE((edge_to_face[11] == std::array<int,2>{4, 5}));
}

TEST_CASE("Hex Edge Centers"){
    std::array<Parfait::Point<double>, 8> hex;
    hex[0] = {0,0,0};
    hex[1] = {1,0,0};
    hex[2] = {1,1,0};
    hex[3] = {0,1,0};
    hex[4] = {0,0,1};
    hex[5] = {1,0,1};
    hex[6] = {1,1,1};
    hex[7] = {0,1,1};

    auto edge_centers = Parfait::CGNS::Hex::computeEdgeCenters(hex);
    REQUIRE(edge_centers.size() == 12);

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

    REQUIRE(edge_centers[8][0] == Approx(0.5));
    REQUIRE(edge_centers[8][1] == Approx(0.0));
    REQUIRE(edge_centers[8][2] == Approx(1.0));

    REQUIRE(edge_centers[9][0] == Approx(1.0));
    REQUIRE(edge_centers[9][1] == Approx(0.5));
    REQUIRE(edge_centers[9][2] == Approx(1.0));

    REQUIRE(edge_centers[10][0] == Approx(0.5));
    REQUIRE(edge_centers[10][1] == Approx(1.0));
    REQUIRE(edge_centers[10][2] == Approx(1.0));

    REQUIRE(edge_centers[11][0] == Approx(0.0));
    REQUIRE(edge_centers[11][1] == Approx(0.5));
    REQUIRE(edge_centers[11][2] == Approx(1.0));
}

TEST_CASE("Hex Face Centers"){

    std::array<Parfait::Point<double>, 8> hex;
    hex[0] = {0,0,0};
    hex[1] = {1,0,0};
    hex[2] = {1,1,0};
    hex[3] = {0,1,0};
    hex[4] = {0,0,1};
    hex[5] = {1,0,1};
    hex[6] = {1,1,1};
    hex[7] = {0,1,1};

    auto face_centers = Parfait::CGNS::Hex::computeFaceCenters(hex);
    REQUIRE(face_centers.size() == 6);
    REQUIRE(face_centers[0][0] == Approx(0.5));
    REQUIRE(face_centers[0][1] == Approx(0.5));
    REQUIRE(face_centers[0][2] == Approx(0.0));

    REQUIRE(face_centers[1][0] == Approx(0.5));
    REQUIRE(face_centers[1][1] == Approx(0.0));
    REQUIRE(face_centers[1][2] == Approx(0.5));

    REQUIRE(face_centers[2][0] == Approx(1.0));
    REQUIRE(face_centers[2][1] == Approx(0.5));
    REQUIRE(face_centers[2][2] == Approx(0.5));

    REQUIRE(face_centers[3][0] == Approx(0.5));
    REQUIRE(face_centers[3][1] == Approx(1.0));
    REQUIRE(face_centers[3][2] == Approx(0.5));

    REQUIRE(face_centers[4][0] == Approx(0.0));
    REQUIRE(face_centers[4][1] == Approx(0.5));
    REQUIRE(face_centers[4][2] == Approx(0.5));

    REQUIRE(face_centers[5][0] == Approx(0.5));
    REQUIRE(face_centers[5][1] == Approx(0.5));
    REQUIRE(face_centers[5][2] == Approx(1.0));
}

