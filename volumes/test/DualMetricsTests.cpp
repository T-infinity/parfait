#include <vector>
#include <catch.hpp>
#include <CGNS.h>
#include <Point.h>
#include <DualMetrics.h>

TEST_CASE("Dual metrics correct") {
    std::array<Parfait::Point<double>, 8> hex;
    hex[0] = {0,0,0};
    hex[1] = {1,0,0};
    hex[2] = {1,1,0};
    hex[3] = {0,1,0};
    hex[4] = {0,0,1};
    hex[5] = {1,0,1};
    hex[6] = {1,1,1};
    hex[7] = {0,1,1};
    using namespace Parfait::CGNS::Hex;

    std::vector<double> volumes;
    std::vector<Parfait::Point<double>> areas;
    tie(volumes, areas) = computeDualMetrics(hex, 8, edge_to_node, edge_to_face,
                                             computeEdgeCenters(hex),
                                             computeFaceCenters(hex),
                                             computeCenter(hex));

    REQUIRE(areas[0][0] == Approx(0.25));
    REQUIRE(areas[0][1] == Approx(0.0));
    REQUIRE(areas[0][2] == Approx(0.0));

    REQUIRE(areas[1][0] == Approx(0.0));
    REQUIRE(areas[1][1] == Approx(0.25));
    REQUIRE(areas[1][2] == Approx(0.0));

    REQUIRE(areas[2][0] == Approx(-0.25));
    REQUIRE(areas[2][1] == Approx(0.0));
    REQUIRE(areas[2][2] == Approx(0.0));

    REQUIRE(areas[3][0] == Approx(0.0));
    REQUIRE(areas[3][1] == Approx(-0.25));
    REQUIRE(areas[3][2] == Approx(0.0));

    for(int i = 4; i < 8; i++){
        REQUIRE(areas[i][0] == Approx(0.0));
        REQUIRE(areas[i][1] == Approx(0.0));
        REQUIRE(areas[i][2] == Approx(0.25));
    }

    for(int i = 0; i < 8; i++){
        REQUIRE(volumes[i] == Approx(0.125));
    }
}
