#include "LTOList.hpp"
#include <chrono>
#include <ctime>
#include <iostream>
#include <thread>

int main()
{
    LTOList<double> ot;
    for (size_t i = 1; i < 100; ++i) {

        for (size_t j = 0; j < 100000; ++j)
            ot.push_back(j);
        std::this_thread::sleep_for(std::chrono::seconds(1));

        std::clock_t cl_begin = std::clock();
        for (size_t k = 0; k < ot.size(); ++k)
            volatile double m = ot[k];
        std::clock_t cl_end = std::clock();
        std::cout << "#Elements " << ot.size() << " Time([1/" << CLOCKS_PER_SEC
                  << "]sec) " << (cl_end - cl_begin) << std::endl;
    }
    return 0;
}
