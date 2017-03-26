#include <catch.hpp>
#include <iostream>
#include <GridSystemPreProcessor.h>



TEST_CASE("GridSystem PreProcessor"){
    std::string input_filename = COMBINED_TEST_JSON;

    auto grid_system = Parfait::importMeshesAsSystem(input_filename);

    if(MessagePasser::Rank() == 1){
        REQUIRE(grid_system.size() == 2);
        REQUIRE(grid_system[0]->numberOfNodes() == 14);
        REQUIRE(grid_system[1]->numberOfNodes() == 14);
    }
}
