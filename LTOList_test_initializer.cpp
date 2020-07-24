#include "LTOList.hpp"

#define CATCH_CONFIG_MAIN
#include <catch.hpp>

TEST_CASE("Constucted by initializer list") {
    LTOList<double> ot = { 1, 5, 9, 7, 8, 2 };
    
    auto it = ot.begin();
    REQUIRE(*it == 1);
    ++it;
    REQUIRE(*it == 5);
    ++it;
    REQUIRE(*it == 9);
    ++it;
    REQUIRE(*it == 7);
    ++it;
    REQUIRE(*it == 8);
    ++it;
    REQUIRE(*it == 2);
    ++it;
    REQUIRE(it == ot.end());
}
