#include "LTOList.hpp"

#define CATCH_CONFIG_MAIN
#include <catch.hpp>

TEST_CASE("Inserting at the beginning") {
    LTOList<int> ot;
    ot.insert(ot.end(), 1);
    ot.insert(ot.end(), 2);
    ot.insert(ot.end(), 3);
    ot.insert(ot.end(), 4);
    ot.insert(ot.end(), 5);
    ot.insert(ot.end(), 6);
    ot.insert(ot.end(), 7);
    ot.insert(ot.end(), 8);

    auto p = ot.begin();
    auto p_begin = p + 3;
    auto p_end = p + 6;
    ot.erase(p_begin, p_end);

    auto it = ot.begin();
    REQUIRE(*it == 1); ++it;
    REQUIRE(*it == 2); ++it;
    REQUIRE(*it == 3); ++it;
    REQUIRE(*it == 7); ++it;
    REQUIRE(*it == 8); ++it;
    REQUIRE(it == ot.end());
}
