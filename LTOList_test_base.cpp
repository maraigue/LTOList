#include "LTOList.hpp"

#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include <iostream>
#include <vector>

bool check_content(typename LTOList<double>::Base & ot, const std::vector<double> & expected){
    using p_node = typename LTOList<double>::pNode;

    p_node pos = ot.origin->o_front();
    auto it = expected.begin();
    for(;;){
        if(pos == ot.origin){
            return it == expected.end();
        }

        if(pos->value != *it) return false;

        pos = ot.neighbor(pos, 1);
        ++it;
    }
}

TEST_CASE("Order of insertions on LTOList::Base") {
    using p_node = typename LTOList<double>::pNode;
    size_t side = 0;

    typename LTOList<double>::Base ot;

    ot.add(ot.origin, 0, 0.5);
    REQUIRE(check_content(ot, {0.5}));

    p_node pos = ot.origin->o_root();
    pos = ot.add(pos, side, 1.5);
    REQUIRE(check_content(ot, {1.5, 0.5}));
    pos = ot.add(pos, 1 - side, 2.5);
    REQUIRE(check_content(ot, {1.5, 2.5, 0.5}));
    pos = ot.add(pos, side, 3.5);
    REQUIRE(check_content(ot, {1.5, 3.5, 2.5, 0.5}));
    pos = ot.add(pos, 1 - side, 4.5);
    REQUIRE(check_content(ot, {1.5, 3.5, 4.5, 2.5, 0.5}));
    pos = ot.add(pos, side, 5.5);
    REQUIRE(check_content(ot, {1.5, 3.5, 5.5, 4.5, 2.5, 0.5}));
    pos = ot.add(pos, 1 - side, 6.5);
    REQUIRE(check_content(ot, {1.5, 3.5, 5.5, 6.5, 4.5, 2.5, 0.5}));
    pos = ot.add(pos, side, 7.5);
    REQUIRE(check_content(ot, {1.5, 3.5, 5.5, 7.5, 6.5, 4.5, 2.5, 0.5}));
}

TEST_CASE("Operations on LTOList::Base") {
    using p_node = typename LTOList<double>::pNode;
    size_t side = 0;

    typename LTOList<double>::Base ot;

    ot.add(ot.origin, 0, 0.5);
    p_node pos = ot.origin->o_root();
    pos = ot.add(pos, side, 1.5);
    pos = ot.add(pos, 1 - side, 2.5);
    pos = ot.add(pos, side, 3.5);
    pos = ot.add(pos, 1 - side, 4.5);
    pos = ot.add(pos, side, 5.5);
    pos = ot.add(pos, 1 - side, 6.5);
    pos = ot.add(pos, side, 7.5);

    SECTION("Reverse iterator"){
        p_node pos = ot.origin->o_back();

        REQUIRE(pos->value == 0.5);
        pos = ot.neighbor(pos, 0);
        REQUIRE(pos->value == 2.5);
        pos = ot.neighbor(pos, 0);
        REQUIRE(pos->value == 4.5);
        pos = ot.neighbor(pos, 0);
        REQUIRE(pos->value == 6.5);
        pos = ot.neighbor(pos, 0);
        REQUIRE(pos->value == 7.5);
        pos = ot.neighbor(pos, 0);
        REQUIRE(pos->value == 5.5);
        pos = ot.neighbor(pos, 0);
        REQUIRE(pos->value == 3.5);
        pos = ot.neighbor(pos, 0);
        REQUIRE(pos->value == 1.5);
        REQUIRE(pos == ot.origin->o_front());
    }

    SECTION("Random access"){
        p_node pos;

        pos = ot.origin->o_front();
        pos = ot.advance(pos, 7);
        REQUIRE(pos->value == 0.5);
        pos = ot.origin->o_front();
        pos = ot.advance(pos, 0);
        REQUIRE(pos->value == 1.5);
        pos = ot.origin->o_front();
        pos = ot.advance(pos, 6);
        REQUIRE(pos->value == 2.5);
        pos = ot.origin->o_front();
        pos = ot.advance(pos, 1);
        REQUIRE(pos->value == 3.5);
        pos = ot.origin->o_front();
        pos = ot.advance(pos, 5);
        REQUIRE(pos->value == 4.5);
        pos = ot.origin->o_front();
        pos = ot.advance(pos, 2);
        REQUIRE(pos->value == 5.5);
        pos = ot.origin->o_front();
        pos = ot.advance(pos, 4);
        REQUIRE(pos->value == 6.5);
        pos = ot.origin->o_front();
        pos = ot.advance(pos, 3);
        REQUIRE(pos->value == 7.5);
    }
}
