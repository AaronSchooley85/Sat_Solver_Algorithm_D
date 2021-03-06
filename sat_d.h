#ifndef SAT_D
#define SAT_D

#include <vector>
#include <unordered_set>
#include <unordered_map>
#include "variable.h"
#include "clause.h"

class sat_d
{
	private:
		// A record of the decisions we've made.
		std::vector<int> moves;	

		// This maps the depth of our search to the variable
		// which was assigned at that depth.
		std::vector<variable*> dv_map;

		// The vectors of variables which is 1-based.
		// Variable k contains positive and negative
		// literals 2k and 2k+1, respectively.  
		std::vector<variable> variables;
		std::vector<clause> clauses; 

		int n{-1}; // Number of variables.
		int m{-1}; // Number of clauses.
	
		variable* head{nullptr};
		variable* tail{nullptr};
		variable* k{nullptr};

	public:
		std::vector<int> solve(std::vector<std::vector<int>>&);
		void Initialize(std::vector<std::vector<int>>&);
		void JoinRing(variable&);
		void BackTrack(int& d);
		void Display();

};

#endif
