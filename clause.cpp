#include "clause.h"
#include "variable.h"

void clause::SetLiterals(std::vector<int>* l)
{
	literals = l;
}

void clause::SetNextClause(clause* c)
{
	next = c;
}

void clause::SetIndex(int i)
{
	index = i;
}

std::vector<int>* clause::GetLiterals()
{
	return literals;
}

clause* clause::GetNextClause()
{
	return next;
}


// Change to a new literal that can be watched because it is not false.
// return false if impossible and perform no change.
void clause::ChangeWatch(std::vector<variable>& variables)
{
	// Get a pointer to the literals in this clause.
	auto lp { GetLiterals() };
	for (auto li {lp->begin()+1}; li < lp->end(); li++)
	{
		// Check if any other literal is watchable.
		auto lit { (*li) };
		variable& var { variables[lit >> 1] };	
		auto val { var.GetValue() };

		// If variable undetermined or in agreement, swap.
		// Return successful indication.
		// Knuth's code doesn't check val negative. Verify.
		if (val < 0 || val != (lit & 1))
		{
			auto temp { *(lp->begin()) };	
			*(lp->begin()) = *li;
			*li = temp;
			var.JoinWatch(*this, ((lit & 1) ? false: true));
			return;
		}
	} 

	// No literals are true. We failed to change.
	std::cout << "Failed to swap. This should never happen" << std::endl;
	std::cout << "There is likely a bug in this solver. Press any key to continue " << std::endl;
	std::cin.get();
}

int clause::GetIndex()
{
	return index;
}
