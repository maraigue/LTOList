#include "LTOList.hpp"
#include <iostream>
#include <utility>

int main(){
	{
		LTOList<std::pair<int, int>> ot;
		ot.emplace_back(1, 3);
		ot.emplace_front(5, 7);
		ot.emplace(1, ot.begin(), 10, 12);

		for(auto it = ot.begin(); it != ot.end(); ++it){
			std::cout << it->first << ":" << it->second << " ";
		}
		std::cout << std::endl;
	}

	return 0;
}
