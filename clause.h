#ifndef CLAUSE_H
#define CLAUSE_H

#include <vector>
class variable; // Resolve circular dependency. 
class clause
{
	private:
		std::vector<int>* literals{nullptr};
		clause* next{nullptr};
		int index{-1};

	public:
		void SetLiterals(std::vector<int>*);
		void SetNextClause(clause*);
		void SetIndex(int);
		void JoinWatch(std::vector<variable>&, int);
		void ChangeWatch(std::vector<variable>&);

		std::vector<int>* GetLiterals();
		clause* GetNextClause();
		int GetIndex();
};

#endif
