#include "LTOList.hpp"

#define CATCH_CONFIG_MAIN
#include <catch.hpp>

TEST_CASE("Copying LTOList::Base") {
    using p_node = typename LTOList<double>::pNode;

    LTOList<double>::Base ot1;

    for(size_t i = 0; i < 16; ++i) {
        LTOList<double>::Base ot2;
        for(size_t j = 0; j < i; ++j)
            ot2.add(ot2.origin->o_back(), 1, j);

        ot1.copy_from(ot2);
        REQUIRE(ot1.size() == i);
        REQUIRE(ot2.size() == i);

        // Check equality
        p_node p1 = ot1.origin->o_front();
        p_node p2 = ot2.origin->o_front();
        for(;;){
            if(p1 == nullptr || p2 == nullptr || p1 == ot1.origin || p2 == ot2.origin) break;
            REQUIRE(p1->value == p2->value);
            p1 = ot1.advance(p1, 1);
            p2 = ot2.advance(p2, 1);
        }
        REQUIRE(((p1 == nullptr && p2 == nullptr) || (p1 == ot1.origin && p2 == ot2.origin)));
    }
}

TEST_CASE("Copying LTOList") {
    LTOList<double> ot1;

    for(size_t i = 0; i < 16; ++i) {
        LTOList<double> ot2;
        for(size_t j = 0; j < i; ++j) ot2.insert(ot2.end(), j);

        LTOList<double> ot1(ot2);

        auto it1 = ot1.begin();
        auto it2 = ot2.begin();
        for(;;){
            if(it1 == ot1.end() || it2 == ot2.end()) break;
            REQUIRE(*it1 == *it2);
            ++it1;
            ++it2;
        }
        REQUIRE(it1 == ot1.end());
        REQUIRE(it2 == ot2.end());
    }
}
