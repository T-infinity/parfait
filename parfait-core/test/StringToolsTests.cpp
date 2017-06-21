#include <catch.hpp>
#include <StringTools.h>


TEST_CASE("String split by delimiter"){
    std::string s = "This is a sentence.";
    auto l = StringTools::split(s, " ");
    REQUIRE(l.size() == 4);
    REQUIRE("This" == l[0]);
    REQUIRE("is" == l[1]);
    REQUIRE("a" == l[2]);
    REQUIRE("sentence." == l[3]);
}

TEST_CASE("String split by string delimiter"){
    std::string s = "(>'-')> <('-'<) ^(' - ')^ <('-'<) (>'-')>";
    auto l = StringTools::split(s, "'-'");
    REQUIRE(l.size() == 5);

}
