#include "LTOList.hpp"
#include <iostream>

int main()
{
    // Test (Base)
    {
        LTOList<double>::Base ot1;

        for (size_t i = 0; i < 16; ++i) {
            LTOList<double>::Base ot2;
            for (size_t j = 0; j < i; ++j)
                ot2.add(ot2.origin->o_back(), 1, j);

            ot1.copy_from(ot2);

            std::cout << "---------- After copy_from()" << std::endl;
            ot1.display_tree(std::cout);
            std::cout << std::endl;
            ot2.display_tree(std::cout);
            std::cout << std::endl;
        }
    }

    // Test (API)
    {
        for (size_t i = 0; i < 16; ++i) {
            LTOList<double> ot2;
            for (size_t j = 0; j < i; ++j)
                ot2.insert(ot2.end(), j);

            LTOList<double> ot1(ot2);

            std::cout << "---------- After copy constructor" << std::endl;
            std::cout << "---------- ot1.end() == " << ot1.end() << std::endl;
            size_t z = 0;
            for (auto it = ot1.begin(); it != ot1.end(); ++it) {
                std::cout << *it << " ";
                ++z;
                if (z > 20) break;
            }
            std::cout << std::endl;

            z = 0;
            for (auto it = ot2.begin(); it != ot2.end(); ++it) {
                std::cout << *it << " ";
                ++z;
                if (z > 20) break;
            }
            std::cout << std::endl;
        }
    }

    return 0;
}
