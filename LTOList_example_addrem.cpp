#include "LTOList.hpp"
#include <iostream>

void test_base() {
    using p_node = typename LTOList<double>::pNode;
    size_t side = 0;

    typename LTOList<double>::Base ot;

    for(size_t num_orig_elems = 2; num_orig_elems <= 16;
        ++num_orig_elems) {
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

            std::cout << "num_orig_elems " << num_orig_elems
                      << " id_rem_elem " << id_rem_elem
                      << " returned "
                      << (ot.remove(pos_removed) == pos_next)
                      << std::endl;
        }
    }
}

void test_api() {
    LTOList<double> ot;

    for(size_t num_orig_elems = 2; num_orig_elems <= 16;
        ++num_orig_elems) {
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

            std::cout << "num_orig_elems " << num_orig_elems
                      << " id_rem_elem " << id_rem_elem
                      << " returned "
                      << (ot.erase(pos_removed) == pos_next)
                      << std::endl;
        }
    }
}

int main() {
    test_base();
    test_api();
}
