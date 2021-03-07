#include <iostream>
#include <unordered_set>
#include <unordered_map>
#include <cmath>
#include "sat_d.h"
#include "clause.h"
#include "variable.h"

// Expects n variables x1 to xn.
// Returns a vector containing literal values with positive integers
// for positive polarity literals and negative integers for negative
// polarity literals. A vector of zero length is returned if the 
// clauses are unsatisfiable. 

// Head points to the variable being processed. Head gets moved through
// the ring looking for unit clauses. 

// k is a temp pointer used to advance head and points to the previous
// variable in the ring so that when we modify the ring we know which
// variable preceeds what head is pointing to and helps us to modify our
// links. 

// h is an array of head positions. At any index i, it tells us which variable
// was processed by head at depth i. 

// tail is used to ensure we walk the entire circular ring. When head equals
// tail we know we've walked the entire linked list. At any time, setting 
// tail to be the element preceeding your new head position ensures the head
// pointer will walk the entire list.
std::vector<int> sat_d::solve(std::vector<std::vector<int>>& input_clauses)
{
	// D1
	// Initialize all variables and vectors of the class.
	Initialize(input_clauses);	

	// Begin the assignment of variables.
	moves[0] = 0;
	int d{0};

	// Keep going while there exist variables being watched.
	while (tail != nullptr)
	{
		k = tail; // set k to the last variable. Always reset at beginning of each loop.

		// This do-while loop serves one purpose: to find unit clauses.
		// f == 1 or f == 2 means a unit clause success. f == 3 indicates
		// a contradiction so we must back track. f == 0 means no unit
		// clauses and to continue while variables remain to check.
		int f{0};
		do
		{
			// Step head forward to next variable.
			head = k->GetNextVariable();

			// Determine if this variable is forced by being in a unit clause.
			f = head->IsUnit(variables,true) + 2 * head->IsUnit(variables,false);

			if ( f == 1 || f == 2)
			{
				// Indicate that one of the polarities of the variable has been forced.
				moves[d+1] = f + 3;
				tail = k;
			}
			// If neither polarity for this variable is forced.
			else if ( f == 0 )
			{
				// Increment k to the next variable object and check
				// if any unit clauses exist for it.
				if (head != tail) k = head;
			}

		}while (f == 0 && f!= 3 && head != tail);

		// Here head == tail and k is right before head.
		// If head == tail, we have walked the entire active ring and
		// found no unit clauses. We must instead branch, essentially 
		// guessing the polarity and investigating the implications.

		// D4
		if (f == 0)
		{
			head = tail->GetNextVariable();
			if (head->GetWatchList(true) == nullptr) moves[d+1] = 1;
			else if (head->GetWatchList(false) != nullptr) moves[d+1] = 1;
			else moves[d+1] = 0; 	
		}

		// Increment the depth to the next variable.
		// Indicate which variable was set at this depth.

		// D5
		if (f != 3)
		{
			d++;
			k = head;
			dv_map[d] = k;
			if (tail == k)
			{
				// Note that updating the watch tables below can cause
				// the ring to be populated again and that watch update routine
				// May change tail to a valid pointer again. 
				tail = nullptr;
			}
			else
			{
				head = k->GetNextVariable();
				tail->SetNextVariable(head);
				variable* p = tail;
			}
		}	
		else // D7
		{
			// Due to a contradiction of unit clauses, we must backtrack.
			BackTrack(d);

			if (d)
			{
				moves[d] = 3 - moves[d];
				k = dv_map[d];
			}
			else
			{
				// Return empty vector if we backtracked to level 0. No solution.
				return std::vector<int>();
			}
		}

		// D6 - Update the watches.
		int b { (moves[d] + 1) % 2 };	
		k->SetValue(b);
	    k->ClearWatchList( variables, b ? false : true ); // Pass inverted logic.
	}		

	std::vector<int> answer(static_cast<int>(variables.size() -1 ), -1);
	int i{0};
	for (auto it {variables.begin() + 1}; it < variables.end(); it++)
	{
		answer[i++] = it->GetValue();
	}

	return answer;
}

// Initialize data structures.
void sat_d::Initialize(std::vector<std::vector<int>> &input_clauses)
{

	// Determine the total number of clauses.
	m = static_cast<int>(input_clauses.size());
	std::cout << m << " clauses." << std::endl;

	// Determine the number of unique variables irrespective of sign.
	for (auto &c : input_clauses)
	{
		for (auto &v : c)
		{
			int a {std::abs(v)};
			if (a > n) n = a;

			// Encode integers to strictly positive natural numbers.
			v = 2 * a + (v < 0);
		}
	}
		
	// Number of variables.	
	std::cout << n << " variables." << std::endl;	

	// Set the size of the vectors which hold the moves we've performed,
	// the literals, and the clauses. Knuth's algorithms use 1-based
	// indexing.
	moves.resize(n+1, 0);
	dv_map.resize(n+1, nullptr);
	variables.resize(n+1);
	clauses.resize(m+1);

	// Set the index field of each variable object as well as sat solver pointer.
	for (std::vector<int>::size_type i{0}; i < variables.size() - 1; i++)
	{
		variables.at(i+1).SetNumber(i+1);
		variables[i+1].SetSat(this);
	}

	// Initialize vector of clauses. 1-based indexing.
	// Simply packing input argument into our clause objects.
	for (std::vector<int>::size_type i{0}; i < input_clauses.size(); i++)
	{
		clauses[i+1].SetLiterals(&input_clauses[i]); // Vector of literals for this clause.		
		clauses[i+1].SetIndex(i+1); // Record the vector index of this clause.
	}

	// Link the variables (both positive and negative literals) and the clauses which watch them.
	for (auto it{clauses.begin()+1}; it < clauses.end(); it++)
	{
		// Get the literal watched by this clause.
		auto lp{it->GetLiterals()}; // Pointer to literal vector.
		auto literal{(*lp)[0]}; // The first element (the watched literal)

		// Have the clause join other clauses in watching this literal.
		variables[literal >> 1].JoinWatch(*it, (literal & 1) ? false : true );
	}	
}

void sat_d::BackTrack(int& d)
{
	tail = k;
	while (moves[d] >= 2)
	{
		k = dv_map[d];
		k->SetValue(-1);
		bool list { k->GetWatchList(true) != nullptr || k->GetWatchList(false) != nullptr};
		if (list)
		{
			k->SetNextVariable(head);
			head = k;
			tail->SetNextVariable(head);
		}
		d--;
	}
}

void sat_d::JoinRing(variable& v)
{
	if (tail == nullptr)
	{
		head = &v;
		tail = head;
		tail->SetNextVariable(head);
	}
	else
	{
		v.SetNextVariable(head);
		head = &v;
		tail->SetNextVariable(head);
	}
}
