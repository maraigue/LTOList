#include "LTOList.hpp"
#include <iostream>
#include <vector>

int main()
{
    // By copying
    {
        LTOList<std::vector<double>> ot;

        std::vector<double> v1 = {2, 3, 5};
        std::vector<double> v2 = {7, 11, 13};

        ot.insert(ot.end(), v1);
        ot.insert(ot.end(), v2);

        std::cout << "ot:";
        for (auto it = ot.begin(); it != ot.end(); ++it) {
            for (auto iv = it->begin(); iv != it->end(); ++iv) {
                std::cout << " " << *iv;
            }
            std::cout << " |";
        }
        std::cout << std::endl;

        std::cout << "v1:";
        for (auto iv = v1.begin(); iv != v1.end(); ++iv) {
            std::cout << " " << *iv;
        }
        std::cout << std::endl;

        std::cout << "v2:";
        for (auto iv = v1.begin(); iv != v1.end(); ++iv) {
            std::cout << " " << *iv;
        }
        std::cout << std::endl;
    }

    // By moving
    {
        LTOList<std::vector<double>> ot;

        std::vector<double> v1 = {2, 3, 5};
        std::vector<double> v2 = {7, 11, 13};

        ot.insert(ot.end(), std::move(v1));
        ot.insert(ot.end(), std::move(v2));

        std::cout << "ot:";
        for (auto it = ot.begin(); it != ot.end(); ++it) {
            for (auto iv = it->begin(); iv != it->end(); ++iv) {
                std::cout << " " << *iv;
            }
            std::cout << " |";
        }
        std::cout << std::endl;

        std::cout << "v1:";
        for (auto iv = v1.begin(); iv != v1.end(); ++iv) {
            std::cout << " " << *iv;
        }
        std::cout << std::endl;

        std::cout << "v2:";
        for (auto iv = v1.begin(); iv != v1.end(); ++iv) {
            std::cout << " " << *iv;
        }
        std::cout << std::endl;
    }

    // By moving (push_front, push_back)
    {
        LTOList<std::vector<double>> ot;

        std::vector<double> v1 = {2, 3, 5};
        std::vector<double> v2 = {7, 11, 13};

        ot.push_back(std::move(v1));
        ot.push_front(std::move(v2));

        std::cout << "ot:";
        for (auto it = ot.begin(); it != ot.end(); ++it) {
            for (auto iv = it->begin(); iv != it->end(); ++iv) {
                std::cout << " " << *iv;
            }
            std::cout << " |";
        }
        std::cout << std::endl;

        std::cout << "v1:";
        for (auto iv = v1.begin(); iv != v1.end(); ++iv) {
            std::cout << " " << *iv;
        }
        std::cout << std::endl;

        std::cout << "v2:";
        for (auto iv = v1.begin(); iv != v1.end(); ++iv) {
            std::cout << " " << *iv;
        }
        std::cout << std::endl;
    }

    return 0;
}
