#include "LTOList.hpp"
#include <iostream>

int main(void)
{
    {
        LTOList<int> ot;
        ot.insert(ot.begin(), 3);
        ot.insert(ot.begin(), 5);
        ot.insert(ot.begin(), 7);
        for (auto it = ot.begin(); it != ot.end(); ++it) {
            std::cout << *it;
        }
        std::cout << std::endl;
    }
    {
        LTOList<int> ot;
        ot.insert(0, ot.begin(), 3);
        ot.insert(0, ot.begin(), 5);
        ot.insert(0, ot.begin(), 7);
        for (auto it = ot.begin(); it != ot.end(); ++it) {
            std::cout << *it;
        }
        std::cout << std::endl;
    }
    {
        LTOList<int> ot;
        ot.insert(1, ot.begin(), 3);
        ot.insert(1, ot.begin(), 5);
        ot.insert(1, ot.begin(), 7);
        for (auto it = ot.begin(); it != ot.end(); ++it) {
            std::cout << *it;
        }
        std::cout << std::endl;
    }
    {
        LTOList<int> ot;
        ot.insert(0, ot.end(), 3);
        ot.insert(0, ot.end(), 5);
        ot.insert(0, ot.end(), 7);
        for (auto it = ot.begin(); it != ot.end(); ++it) {
            std::cout << *it;
        }
        std::cout << std::endl;
    }
    {
        LTOList<int> ot;
        ot.insert(ot.end(), 3);
        ot.insert(ot.end(), 5);
        ot.insert(ot.end(), 7);
        for (auto it = ot.begin(); it != ot.end(); ++it) {
            std::cout << *it;
        }
        std::cout << std::endl;
    }
    {
        LTOList<int> ot;
        ot.insert(ot.end(), 1);
        ot.insert(ot.end(), 2);
        ot.insert(ot.end(), 3);
        ot.insert(ot.end(), 4);
        ot.insert(ot.end(), 5);
        ot.insert(ot.end(), 6);
        ot.insert(ot.end(), 7);
        ot.insert(ot.end(), 8);
        for (size_t i = 0; i < ot.size(); ++i) {
            std::cout << ot[i];
        }
        std::cout << std::endl;

        auto p = ot.begin();
        auto p_begin = p + 3;
        auto p_end = p + 6;
        ot.erase(p_begin, p_end);
        for (size_t i = 0; i < ot.size(); ++i) {
            std::cout << ot[i];
        }
        std::cout << std::endl;
    }
}
