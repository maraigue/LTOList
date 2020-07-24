#include "LTOList.hpp"

#define CATCH_CONFIG_MAIN
#include <catch.hpp>

TEST_CASE("Clearing an LTOList::Base") {
    using p_node = typename LTOList<double>::pNode;
    
    typename LTOList<double>::Base ot;

    for(size_t num_orig_elems = 2; num_orig_elems <= 16; ++num_orig_elems) {
        for(size_t i = 0; i < num_orig_elems; ++i) {
            p_node pos = ot.add(ot.origin->o_back(), 1, i);
        }

        ot.clear();
        REQUIRE(ot.size() == 0);
        REQUIRE(ot.empty());
    }
}

TEST_CASE("Clearing an LTOList") {
    LTOList<double> ot;

    for(size_t num_orig_elems = 2; num_orig_elems <= 16; ++num_orig_elems) {
        for(size_t i = 0; i < num_orig_elems; ++i) {
            ot.insert(ot.end(), i);
        }
        ot.clear();
        REQUIRE(ot.size() == 0);
        REQUIRE(ot.empty());
        REQUIRE(ot.begin() == ot.end());
    }
}
