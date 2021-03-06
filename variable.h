#ifndef VARIABLE_H
#define VARIABLE_H

#include <iostream>
#include <vector>

class sat_d;
class clause; // forward declaration to resolve circular dependency. 
class variable
{

	private:
		int number{-1}; // Variable number
		int value{-1}; // -1 unset, 0 false, 1 true.
		clause* true_list{nullptr};
		clause* false_list{nullptr};
		sat_d* sat{nullptr};
		variable* next_variable{nullptr};
		
	public:
		void SetNumber(int);
		void SetSat(sat_d*);
		void SetValue(int);
		void SetWatchList(clause*, bool);
		void ClearWatchList(std::vector<variable>&, bool);
		void SetNextVariable(variable*);
		void JoinWatch(clause& c, bool polarity);

		int GetNumber();
		int GetValue();
		clause* GetWatchList(bool);
		variable* GetNextVariable();
		int IsUnit(std::vector<variable>&, bool);
};

#endif
