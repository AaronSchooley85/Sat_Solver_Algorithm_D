#include "sat_d.h"
#include <vector>
#include <iostream>

int main()
{
	std::vector<std::vector<int>> clauses {{1,2,-3},{2,3,-4},{3,4,1},{4,-1,2},{-1,2,3},{-2,-3,4},{-3,-4,-1},{-4,1,-2}};
	sat_d s;
	auto answer = s.solve(clauses);
	if (answer.size())
	{
		int c{1};
		for (auto it { answer.begin()}; it < answer.end(); it++)
		{
			if (*it == 1) std::cout << "Variable " << c++ << ": True" << std::endl;
			else if (*it == 0) std::cout << "Variable " << c++ << ": False" << std::endl;
			else if (*it == -1) std::cout << "Variable " << c++ << ": Free" << std::endl;
			else std::cout << "There was an error. Results invalid" << std::endl;
		}
	}
	else
	{
		std::cout<< "No solution exists" << std::endl;
	}

	return 0;
}
