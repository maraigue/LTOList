#include "LTOList.hpp"
#include <iostream>

void test_base()
{
    using p_node = typename LTOList<double>::pNode;
    size_t side = 0;

    typename LTOList<double>::Base ot;

    for (size_t num_orig_elems = 2; num_orig_elems <= 16; ++num_orig_elems) {
        for (size_t i = 0; i < num_orig_elems; ++i) {
            p_node pos = ot.add(ot.origin->o_back(), 1, i);
        }

        std::cout << "---------- Before clear()" << std::endl;
        ot.display_tree(std::cout);
        std::cout << std::endl;

        ot.clear();
        std::cout << "---------- After clear()" << std::endl;
        ot.display_tree(std::cout);
        std::cout << std::endl;
    }
}

void test_api()
{
    LTOList<double> ot;

    for (size_t num_orig_elems = 2; num_orig_elems <= 16; ++num_orig_elems) {
        for (size_t i = 0; i < num_orig_elems; ++i) {
            ot.insert(ot.end(), i);
        }

        std::cout << "---------- Before clear()" << std::endl;
        for (auto it = ot.begin(); it != ot.end(); ++it)
            std::cout << *it << " ";
        std::cout << std::endl;

        ot.clear();
        std::cout << "---------- After clear()" << std::endl;
        for (auto it = ot.begin(); it != ot.end(); ++it)
            std::cout << *it << " ";
        std::cout << std::endl;
    }
}

int main()
{
    test_base();
    test_api();
}
