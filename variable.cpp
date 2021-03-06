#include "variable.h"
#include "sat_d.h"
#include "clause.h"
#include <iostream>

void variable::SetNumber(int n)
{
	number = n;
}

void variable::SetValue(int b)
{
	value = b;
}

void variable::SetSat(sat_d* s)
{
	sat = s;
}

void variable::SetWatchList(clause* c, bool b)
{
	bool empty_lists { true_list == nullptr && false_list == nullptr };
	if (b) true_list = c;	
	else false_list = c;

	// If this pointer is being set and we're not yet in the active ring, add ourselves. 
	if (c != nullptr && (empty_lists && GetValue() < 0)) sat->JoinRing(*this);	
}

// Clear the watch list for the polarity of the variable's value.
void variable::ClearWatchList(std::vector<variable>& variables, bool b)
{
	// Get a pointer to the first clause.
	auto cp { GetWatchList(b)};
	SetWatchList(nullptr, b );
	while (cp != nullptr)
	{ 
		auto nc {cp->GetNextClause()};
		cp->ChangeWatch(variables);
		cp = nc;
	}
}

void variable::SetNextVariable(variable* c)
{
	next_variable = c;
}

int variable::GetNumber()
{
	return number;
}

int variable::GetValue()
{
	return value;
}

clause* variable::GetWatchList(bool b)
{
	return b ? true_list : false_list;
}

variable* variable::GetNextVariable()
{
	return next_variable;
}

// Determine if a variable is a member of any unit clause.
// If it is, it's value is forced.
int variable::IsUnit(std::vector<variable>& v, bool polarity)
{
	// Get the pointer to the linked list of clauses which watch this literal.
	auto cp { GetWatchList(polarity) };

	// Walk the linked list of clauses, checking each of them.
	while (cp != nullptr)
	{
		// Get a pointer to the vector of other literals which the clause contains.
		auto lp { cp->GetLiterals() };

		// Iterate through all the literals and determine if all are false.
		bool unit { true };
		for (auto it {lp->begin() + 1}; it < lp->end() && unit; it++)
		{
			int literal {*it};	
			int variable_number { literal >> 1 };
			auto &var{ v[variable_number] };

			// If the variable is not yet set, or in agreement with the literal.
			if ( var.GetValue() < 0 || var.GetValue() != (literal & 1))
			{
				unit = false;
			}
		}  
	
		// If we got through a whole clause of all false literals.
		if (unit) return 1;

		// Otherwise, get the next clause in the list.
		cp = cp->GetNextClause();
	}

	// If all clauses failed to be unit clauses.
	return 0;
}

void variable::JoinWatch(clause& c, bool polarity)
{
	// We must add the current clause to the list of clauses which watch this literal.
	// Get the correct list based on literal polarity and prepend the supplied clause.
	auto wlp = GetWatchList(polarity);
	c.SetNextClause(wlp);
	SetWatchList(&c,polarity);
}
