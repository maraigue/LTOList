#include "LTOList.hpp"
#include <iostream>

int main()
{
    // Test (Base)
    LTOList<double> ot = {1, 5, 9, 7, 8, 2};
    for (auto it = ot.begin(); it != ot.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    return 0;
}
