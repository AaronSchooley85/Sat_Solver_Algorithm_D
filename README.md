# Sat_Solver_Algorithm_D
This is a satisfiability solver based on Donald Knuth's "algorithm D" in his book "The Art of Computer Programming, Volume 4, fascicle 6".

I am far from an expert on satisfiability solvers but I thought that others could benefit from my efforts to learn about them via Knuth's
books. This algorithm, "algorithm D" is designed for smaller problems. Larger problems should be solved with a more advanced algorithm. 
My hope is to implement a more sophisticated algorithm and post that as well. I hope that this small program will help to improve the lives
of others. Take this code and do what you will with it provided it helps the sum total of humanity in some way and to any degree.

What is a sat solver? Given a propositional logic function, a sat solver will determine the true or false assignment of each variable in
the function so that it evaluates to true. The power in this is that most any problem can be converted to propositional logic and therefore
a sat solver can solve that problem. 

Sat solvers typically take their propositional logic in CNF form. CNF is "Conjuctive Normal Form", perhaps better known as an "and of ors". 

Here is an example:
(x1 || x2) && (^x2 || x3 || x4) && ...

This consists of "clauses", which are the parenthesis, and literals, which are the various xk for all k. 
What is a literal? Well, let's take one of the variables above as an example. x2 is a variable. It can be set to either true or false.
There are therefore two literal values for x2, the positive polarity (true) and negative polarity (false). In an abuse of notation, x2 is
the symbol for the variable x2 and it's positive literal form. ^x2 is the negative polarity of the variable x2.
The objective here would be to set each of xk to either true or false such that every clause is satisfied, just meaning the whole clause is true. 

So you see that in each clause, since it consists exclusively of "or" operators, needs only 1 variable to be true.

A more elegant way to express the CNF above is by doing away with the "x" and just representing each variable by its index integer.

(1 || 2) && (^2 || 3 || 4) && ...

Or since we know that we exclusively deal with the 'or' connective in clauses, we can stop writing those. And We know that all the clauses
use the 'and' connective.
(1,2),(^2,3,4),...

And we don't like the carret symbol so let's just make the negative integer -k, the negative polarity of variable k.
(1,2),(-2,3,4)...

Lastly, order doesn't matter within a clause, nor does the order of clauses matter. Let's then deal with sets.
{ {1,2},{-2,3,4}, ... }


My solver implementation:

I wrote this in c++ and as such, I used vectors as they are the bread and butter of much of c++ (at least it seems that way to me). Vectors are arrays so we may 
wonder why I went on and on about sets up above since now I'm using a data structure which preserves order. Don't trouble yourself with that detail. It was
merely for c++ convenience. 

You pass your problem to the solver in a vector of vectors. The inner vectors hold integers, which hold the literals, and the outer vector holds those vectors.

So here is how the above example is given to my solver:

std::vector<std::vector<int>> CNF { {1,2}, {-2,3,4}, ... }; // Create the vector of vectors holding the literals in your problem. Note: variables are 1-based.
sat_d s; // Instantiate the solver.
std::vector<int> solution = s.solve(CNF); // Pass CNF to solve method. A vector of ints is returned indicating the boolean values.

Note: The variables are 1-based. The first variable is called x1, or just 1. The returned solution vector is zero based. The value of x1, or 1, is in solution[0].
An empty return vector indicates that there is no satisfying combination of literals. I.e. There is no solution.
0 indicates the variable is set to false.
1 indicates the variable is set to true.
-1 indicates that it is a free variable and it doesn't matter what it is set to. 



main.cpp contains sample driver code to demonstrate this functionality.


