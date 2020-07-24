#include "LTOList.hpp"

#define CATCH_CONFIG_MAIN
#include <catch.hpp>

TEST_CASE("Inserting at the beginning") {
    SECTION("Before the beginning"){
        LTOList<int> ot;
        ot.insert(0, ot.begin(), 3);
        ot.insert(0, ot.begin(), 5);
        ot.insert(0, ot.begin(), 7);
        auto it = ot.begin();
        REQUIRE(*it == 7); ++it;
        REQUIRE(*it == 5); ++it;
        REQUIRE(*it == 3); ++it;
        REQUIRE(it == ot.end());
    }

    SECTION("After the beginning"){
        LTOList<int> ot;
        ot.insert(1, ot.begin(), 3);
        ot.insert(1, ot.begin(), 5);
        ot.insert(1, ot.begin(), 7);
        auto it = ot.begin();
        REQUIRE(*it == 3); ++it;
        REQUIRE(*it == 7); ++it;
        REQUIRE(*it == 5); ++it;
        REQUIRE(it == ot.end());
    }

    SECTION("Default behavior"){
        LTOList<int> ot;
        ot.insert(ot.begin(), 3);
        ot.insert(ot.begin(), 5);
        ot.insert(ot.begin(), 7);
        auto it = ot.begin();
        REQUIRE(*it == 7); ++it;
        REQUIRE(*it == 5); ++it;
        REQUIRE(*it == 3); ++it;
        REQUIRE(it == ot.end());
    }
}

TEST_CASE("Inserting at the ending") {
    SECTION("Before the ending"){
        LTOList<int> ot;
        ot.insert(0, ot.end(), 3);
        ot.insert(0, ot.end(), 5);
        ot.insert(0, ot.end(), 7);
        auto it = ot.begin();
        REQUIRE(*it == 3); ++it;
        REQUIRE(*it == 5); ++it;
        REQUIRE(*it == 7); ++it;
        REQUIRE(it == ot.end());
    }

    SECTION("After the ending"){
        LTOList<int> ot;
        
        try{
            ot.insert(1, ot.end(), 3);
            // The first insertion does not throw an exception with the current implementation,
            // but this specification may change in the future
        }catch(const std::invalid_argument &){
        }

        REQUIRE_THROWS_AS(ot.insert(1, ot.end(), 5), std::invalid_argument);
        // The second insertion throws an exception if inserting after end()
    }

    SECTION("Default behavior"){
        LTOList<int> ot;
        ot.insert(ot.end(), 3);
        ot.insert(ot.end(), 5);
        ot.insert(ot.end(), 7);
        auto it = ot.begin();
        REQUIRE(*it == 3); ++it;
        REQUIRE(*it == 5); ++it;
        REQUIRE(*it == 7); ++it;
        REQUIRE(it == ot.end());
    }
}
