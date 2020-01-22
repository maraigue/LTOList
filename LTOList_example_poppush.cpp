#include "LTOList.hpp"
#include <iostream>

int main() {
    {
        LTOList<double> ot;
        for(size_t i = 0; i < 7; ++i) ot.insert(ot.end(), i);

        std::cout << "---------- Before pop_front()" << std::endl;
        for(auto it = ot.begin(); it != ot.end(); ++it)
            std::cout << *it << " ";
        std::cout << std::endl;

        ot.pop_front();

        std::cout << "---------- After pop_front()" << std::endl;
        for(auto it = ot.begin(); it != ot.end(); ++it)
            std::cout << *it << " ";
        std::cout << std::endl;
    }

    {
        LTOList<double> ot;
        for(size_t i = 0; i < 7; ++i) ot.insert(ot.end(), i);

        std::cout << "---------- Before pop_back()" << std::endl;
        for(auto it = ot.begin(); it != ot.end(); ++it)
            std::cout << *it << " ";
        std::cout << std::endl;

        ot.pop_back();

        std::cout << "---------- After pop_back()" << std::endl;
        for(auto it = ot.begin(); it != ot.end(); ++it)
            std::cout << *it << " ";
        std::cout << std::endl;
    }

    {
        LTOList<double> ot;
        for(size_t i = 0; i < 7; ++i) ot.insert(ot.end(), i);

        std::cout << "---------- Before push_front()" << std::endl;
        for(auto it = ot.begin(); it != ot.end(); ++it)
            std::cout << *it << " ";
        std::cout << std::endl;

        ot.push_front(100);

        std::cout << "---------- After push_front()" << std::endl;
        for(auto it = ot.begin(); it != ot.end(); ++it)
            std::cout << *it << " ";
        std::cout << std::endl;
    }

    {
        LTOList<double> ot;
        for(size_t i = 0; i < 7; ++i) ot.insert(ot.end(), i);

        std::cout << "---------- Before push_back()" << std::endl;
        for(auto it = ot.begin(); it != ot.end(); ++it)
            std::cout << *it << " ";
        std::cout << std::endl;

        ot.push_back(100);

        std::cout << "---------- After push_back()" << std::endl;
        for(auto it = ot.begin(); it != ot.end(); ++it)
            std::cout << *it << " ";
        std::cout << std::endl;
    }

    return 0;
}
