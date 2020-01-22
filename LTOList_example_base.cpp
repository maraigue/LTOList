#include "LTOList.hpp"
#include <iostream>

int main(void) {
    using p_node = typename LTOList<double>::pNode;
    size_t side = 0;

    typename LTOList<double>::Base ot;

    std::cout << "----------------------------" << std::endl;
    std::cout << "Insertion" << std::endl;
    std::cout << "----------------------------" << std::endl;

    ot.add(ot.origin, 0, 0.5);
    ot.display_tree(std::cout);
    std::cout << std::endl;

    p_node pos = ot.origin->o_root();

    pos = ot.add(pos, side, 1.5);
    ot.display_tree(std::cout);
    std::cout << std::endl;
    pos = ot.add(pos, 1 - side, 2.5);
    ot.display_tree(std::cout);
    std::cout << std::endl;
    pos = ot.add(pos, side, 3.5);
    ot.display_tree(std::cout);
    std::cout << std::endl;
    pos = ot.add(pos, 1 - side, 4.5);
    ot.display_tree(std::cout);
    std::cout << std::endl;
    pos = ot.add(pos, side, 5.5);
    ot.display_tree(std::cout);
    std::cout << std::endl;
    pos = ot.add(pos, 1 - side, 6.5);
    ot.display_tree(std::cout);
    std::cout << std::endl;
    pos = ot.add(pos, side, 7.5);
    ot.display_tree(std::cout);
    std::cout << std::endl;

    std::cout << "----------------------------" << std::endl;
    std::cout << "Iteration (front)" << std::endl;
    std::cout << "----------------------------" << std::endl;
    pos = ot.origin->o_front();
    for(size_t i = 0; i < 25; ++i) {
        if(pos == ot.origin) {
            std::cout << "END";
        } else {
            std::cout << pos->value;
        }
        std::cout << " Position=" << ot.position(pos) << std::endl;
        pos = ot.neighbor(pos, 1);
    }

    std::cout << "----------------------------" << std::endl;
    std::cout << "Iteration (back)" << std::endl;
    std::cout << "----------------------------" << std::endl;
    pos = ot.origin->o_back();
    for(size_t i = 0; i < 25; ++i) {
        if(pos == nullptr) break;

        if(pos == ot.origin) {
            std::cout << "END";
        } else {
            std::cout << pos->value;
        }
        std::cout << " Position=" << ot.position(pos) << std::endl;
        pos = ot.neighbor(pos, 0);
    }

    std::cout << "----------------------------" << std::endl;
    std::cout << "Random access (size == " << ot.size() << ")"
              << std::endl;
    std::cout << "----------------------------" << std::endl;
    for(int i = 0; i < ot.size(); ++i) {
        pos = ot.origin->o_front();
        pos = ot.advance(pos, i);
        std::cout << "[" << i << "]" << std::endl;
        for(int j = -(int)(ot.size()); j <= (int)(ot.size()); ++j) {
            std::cout << "[" << i << "] ";
            if(pos == ot.origin) {
                std::cout << "END";
            } else {
                std::cout << pos->value;
            }

            p_node pos2 = ot.advance(pos, j);
            std::cout << " [" << (i + j) << "] ";
            if(pos2 == ot.origin) {
                std::cout << "END";
            } else {
                std::cout << pos2->value;
            }
            std::cout << std::endl;
        }
    }

    std::cout << "----------------------------" << std::endl;
    std::cout << "Removal" << std::endl;
    std::cout << "----------------------------" << std::endl;
    while(ot.size() > 0) {
        std::cout
          << "=========================RESULT========================="
          << std::endl;
        ot.display_tree(std::cout);
        std::cout << std::endl;

        // p_node removing = ot.origin->o_front();
        p_node removing = ot.origin->o_root();
        // p_node removing = ot.origin->o_back();
        p_node next_to_removed = ot.remove(removing);
        std::cout << std::endl;
        std::cout << "REMOVING " << removing << " -- next_to_removed "
                  << next_to_removed << std::endl;
        std::cout
          << "========================================================"
          << std::endl;
    }
}
