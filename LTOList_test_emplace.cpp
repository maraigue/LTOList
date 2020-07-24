#include "LTOList.hpp"
#include <utility>

#define CATCH_CONFIG_MAIN
#include <catch.hpp>

TEST_CASE("Emplacing elements") {
    LTOList<std::pair<int, int>> ot;
    ot.emplace_back(1, 3);
    ot.emplace_front(5, 7);
    ot.emplace(1, ot.begin(), 10, 12);

    REQUIRE(ot[0].first == 5);
    REQUIRE(ot[1].first == 10);
    REQUIRE(ot[2].first == 1);
    REQUIRE(ot[0].second == 7);
    REQUIRE(ot[1].second == 12);
    REQUIRE(ot[2].second == 3);
}
