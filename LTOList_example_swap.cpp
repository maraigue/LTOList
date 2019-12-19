#include "LTOList.hpp"
#include <iostream>

int main(){
	// Test of swap
	{
		LTOList<double> ot1, ot2;

		for(size_t i = 0; i < 4; ++i) ot1.insert(ot1.end(), i);
		for(size_t i = 0; i < 9; ++i) ot2.insert(ot2.end(), i);

		std::cout << "---------- Before swap()" << std::endl;
		for(auto it = ot1.begin(); it != ot1.end(); ++it) std::cout << *it << " ";
		std::cout << std::endl;
		for(auto it = ot2.begin(); it != ot2.end(); ++it) std::cout << *it << " ";
		std::cout << std::endl;

		ot1.swap(ot2);

		std::cout << "---------- After swap()" << std::endl;
		for(auto it = ot1.begin(); it != ot1.end(); ++it) std::cout << *it << " ";
		std::cout << std::endl;
		for(auto it = ot2.begin(); it != ot2.end(); ++it) std::cout << *it << " ";
		std::cout << std::endl;
	}

	// Test of move constructor
	{
		LTOList<double> ot1;
		for(size_t i = 0; i < 4; ++i) ot1.insert(ot1.end(), i);

		std::cout << "---------- Before move construction" << std::endl;
		for(auto it = ot1.begin(); it != ot1.end(); ++it) std::cout << *it << " ";
		std::cout << std::endl;

		LTOList<double> ot2(std::move(ot1));
		std::cout << "---------- After move construction" << std::endl;
		for(auto it = ot1.begin(); it != ot1.end(); ++it) std::cout << *it << " ";
		std::cout << std::endl;
		for(auto it = ot2.begin(); it != ot2.end(); ++it) std::cout << *it << " ";
		std::cout << std::endl;
	}

	return 0;
}
