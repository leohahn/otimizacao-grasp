#ifndef __WPMAXSAT_H__

#include "grasp.h"
#include <vector>
#include <string>
#include <algorithm>
//!
//! Class that implements the GRASP algorithm with the
//! Weighted Partial Max SAT problem.
//!
class WpMaxSAT : public GRASP {
public:
    WpMaxSAT(std::string);
    ~WpMaxSAT();
    void run(int max_iterations);
    void constructGreddyRandomSolution();
    void makeLocalSearch();
    void updateSolution();

    enum ClauseType {SOFT, HARD};

private:
    //!
    //! Returns the number of satisfied clauses given the type (HARD, SOFT)
    //! the variable and its value (0, 1).
    //!
    int numOfSatisfiedClauses(int var, int var_value, ClauseType type);
    int getNumVariables();
    int findInClause(int clause, int var, ClauseType type);

    std::vector<int> rcl;
    void parseFile(std::string path);
    //variable values start from index 1
    //index 0 is ignored
    std::vector<int> variables;
    int numVariables;
    std::vector<std::vector<int> > hardClauses;
    std::vector<std::vector<int> > softClauses;

    std::vector<int> solution;
    int top;
};

#endif // __WPMAXSAT_H__
