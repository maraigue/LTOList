#include "LTOList.hpp"
#include <sstream>

#define CATCH_CONFIG_MAIN
#include <catch.hpp>

TEST_CASE("Removing an element in the LTOList::Base class") {
    using p_node = typename LTOList<double>::pNode;
    size_t side = 0;

    typename LTOList<double>::Base ot;

    for(size_t num_orig_elems = 2; num_orig_elems <= 16; ++num_orig_elems) {
        std::stringstream test_case_name;
        test_case_name << "Removing an element from " << num_orig_elems <<"-element LTOList::Base returns the next element to the removed one";
        SECTION(test_case_name.str()){
            for(size_t id_rem_elem = 0; id_rem_elem < num_orig_elems;
                ++id_rem_elem) {
                p_node pos_removed = nullptr;
                p_node pos_next = ot.origin;
                for(size_t i = 0; i < num_orig_elems; ++i) {
                    p_node pos = ot.add(ot.origin->o_back(), 1, i);
                    if(i == id_rem_elem) {
                        pos_removed = pos;
                    } else if(i == id_rem_elem + 1) {
                        pos_next = pos;
                    }
                }
                REQUIRE(ot.remove(pos_removed) == pos_next);
            }
        }
    }
}

TEST_CASE("Removing an element in the LTOList class") {
    LTOList<double> ot;

    for(size_t num_orig_elems = 2; num_orig_elems <= 16; ++num_orig_elems) {
        std::stringstream test_case_name;
        test_case_name << "Removing an element from " << num_orig_elems <<"-element LTOList returns the next element to the removed one";
        SECTION(test_case_name.str()){
            for(size_t id_rem_elem = 0; id_rem_elem < num_orig_elems;
                ++id_rem_elem) {
                LTOList<double>::iterator pos_removed;
                LTOList<double>::iterator pos_next = ot.end();
                for(size_t i = 0; i < num_orig_elems; ++i) {
                    LTOList<double>::iterator pos =
                    ot.insert(ot.end(), i);
                    if(i == id_rem_elem) {
                        pos_removed = pos;
                    } else if(i == id_rem_elem + 1) {
                        pos_next = pos;
                    }
                }
                REQUIRE(ot.erase(pos_removed) == pos_next);
            }
        }
    }
}

