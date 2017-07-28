#include <catch.hpp>
#include <parfait/CartBlock.h>
#include <parfait/NodeToCell.h>

using namespace Parfait;

TEST_CASE("Build Node To Cell from a MeshTemplate"){
	CartBlock block({0,0,0},{1,1,1},2,2,2);
	auto n2c = buildNodeToCell(block);

	REQUIRE(block.numberOfNodes() == n2c.size());
	REQUIRE(1 == n2c[0].size());
	REQUIRE(0 == n2c[0][0]);
	REQUIRE(2 == n2c[1].size());
}
