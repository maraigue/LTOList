#include "LTOList.hpp"

#define CATCH_CONFIG_MAIN
#include <catch.hpp>

TEST_CASE("Swapping LTOList"){
    LTOList<double> ot1, ot2;

    for(size_t i = 0; i < 4; ++i) ot1.insert(ot1.end(), i);
    for(size_t i = 0; i < 9; ++i) ot2.insert(ot2.end(), i);

    SECTION("Before swapping"){
        REQUIRE(ot1.size() == 4);
        REQUIRE(ot1[0] == 0);
        REQUIRE(ot1[1] == 1);
        REQUIRE(ot1[2] == 2);
        REQUIRE(ot1[3] == 3);
        REQUIRE(ot2.size() == 9);
        REQUIRE(ot2[0] == 0);
        REQUIRE(ot2[1] == 1);
        REQUIRE(ot2[2] == 2);
        REQUIRE(ot2[3] == 3);
        REQUIRE(ot2[4] == 4);
        REQUIRE(ot2[5] == 5);
        REQUIRE(ot2[6] == 6);
        REQUIRE(ot2[7] == 7);
        REQUIRE(ot2[8] == 8);
    }

    ot1.swap(ot2);

    SECTION("After swapping"){
        REQUIRE(ot2.size() == 4);
        REQUIRE(ot2[0] == 0);
        REQUIRE(ot2[1] == 1);
        REQUIRE(ot2[2] == 2);
        REQUIRE(ot2[3] == 3);
        REQUIRE(ot1.size() == 9);
        REQUIRE(ot1[0] == 0);
        REQUIRE(ot1[1] == 1);
        REQUIRE(ot1[2] == 2);
        REQUIRE(ot1[3] == 3);
        REQUIRE(ot1[4] == 4);
        REQUIRE(ot1[5] == 5);
        REQUIRE(ot1[6] == 6);
        REQUIRE(ot1[7] == 7);
        REQUIRE(ot1[8] == 8);
    }
}

TEST_CASE("Constructing LTOList by moving"){
    LTOList<double> ot1;
    for(size_t i = 0; i < 4; ++i) ot1.insert(ot1.end(), i);

    SECTION("Before moving"){
        REQUIRE(ot1.size() == 4);
        REQUIRE(ot1[0] == 0);
        REQUIRE(ot1[1] == 1);
        REQUIRE(ot1[2] == 2);
        REQUIRE(ot1[3] == 3);
    }

    LTOList<double> ot2(std::move(ot1));

    SECTION("After moving"){
        REQUIRE(ot2.size() == 4);
        REQUIRE(ot2[0] == 0);
        REQUIRE(ot2[1] == 1);
        REQUIRE(ot2[2] == 2);
        REQUIRE(ot2[3] == 3);
        REQUIRE(ot1.size() == 0);
    }
}
