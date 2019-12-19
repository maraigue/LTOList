#include "LTOList.hpp"
#include <iostream>
#include <vector>

int main(){
	std::vector<double> m;
	for(size_t i = 100; i < 105; ++i) m.push_back(i);

	for(size_t k = 0; k < 7; ++k){
		LTOList<double> ot;
		
		LTOList<double>::iterator target;
		for(size_t i = 0; i < 7; ++i){
			LTOList<double>::iterator target_tmp = ot.insert(ot.end(), i);
			if(i == k) target = target_tmp;
		}

		std::cout << "---------- Before insert()" << std::endl;
		for(auto it = ot.begin(); it != ot.end(); ++it) std::cout << *it << " ";
		std::cout << std::endl;

		ot.insert(target, m.begin(), m.end());

		std::cout << "---------- After insert()" << std::endl;
		for(auto it = ot.begin(); it != ot.end(); ++it) std::cout << *it << " ";
		std::cout << std::endl;
	}

	return 0;
}
