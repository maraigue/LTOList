#include "LTOList.hpp"
#include <vector>

#define CATCH_CONFIG_MAIN
#include <catch.hpp>

TEST_CASE("Inserting elements by copying") {
    LTOList<std::vector<double>> ot;

    std::vector<double> v1 = { 2, 3, 5 };
    std::vector<double> v2 = { 7, 11, 13, 17 };

    ot.insert(ot.end(), v1);
    ot.insert(ot.end(), v2);

    REQUIRE(ot.size() == 2);
    REQUIRE(ot[0][0] == 2);
    REQUIRE(ot[0][1] == 3);
    REQUIRE(ot[0][2] == 5);
    REQUIRE(ot[1][0] == 7);
    REQUIRE(ot[1][1] == 11);
    REQUIRE(ot[1][2] == 13);
    REQUIRE(ot[1][3] == 17);

    REQUIRE(v1.size() == 3);
    REQUIRE(v2.size() == 4);
}

TEST_CASE("Inserting elements by moving") {
    LTOList<std::vector<double>> ot;

    std::vector<double> v1 = { 2, 3, 5 };
    std::vector<double> v2 = { 7, 11, 13, 17 };

    ot.insert(ot.end(), std::move(v1));
    ot.insert(ot.end(), std::move(v2));

    REQUIRE(ot.size() == 2);
    REQUIRE(ot[0][0] == 2);
    REQUIRE(ot[0][1] == 3);
    REQUIRE(ot[0][2] == 5);
    REQUIRE(ot[1][0] == 7);
    REQUIRE(ot[1][1] == 11);
    REQUIRE(ot[1][2] == 13);
    REQUIRE(ot[1][3] == 17);

    REQUIRE(v1.size() == 0);
    REQUIRE(v2.size() == 0);
}

TEST_CASE("Inserting elements by moving (push_front, push_back)") {
    LTOList<std::vector<double>> ot;

    std::vector<double> v1 = { 2, 3, 5 };
    std::vector<double> v2 = { 7, 11, 13, 17 };

    ot.push_back(std::move(v1));
    ot.push_front(std::move(v2));

    REQUIRE(ot.size() == 2);
    REQUIRE(ot[0][0] == 7);
    REQUIRE(ot[0][1] == 11);
    REQUIRE(ot[0][2] == 13);
    REQUIRE(ot[0][3] == 17);
    REQUIRE(ot[1][0] == 2);
    REQUIRE(ot[1][1] == 3);
    REQUIRE(ot[1][2] == 5);

    REQUIRE(v1.size() == 0);
    REQUIRE(v2.size() == 0);
}
