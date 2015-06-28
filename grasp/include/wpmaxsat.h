#ifndef __WPMAXSAT_H__

#include "grasp.h"
#include <vector>
#include <algorithm>
//!
//! Class that implements the GRASP algorithm with the
//! Weighted Partial Max SAT problem.
//!
class WpMaxSAT : public GRASP {
public:
    WpMaxSAT();
    ~WpMaxSAT();
    void run(int max_iterations);
    void constructGreddyRandomSolution();
    void makeLocalSearch();
    void updateSolution();

    enum ClauseType {SOFT, HARD};
private:

    int numOfSatisfiedClauses(int var, int var_value, ClauseType type);
    int getNumVariables();
    int findInClause(int clause, int var, ClauseType type);

    std::vector<int> rcl;
    std::vector<int> variables;
    std::vector<std::vector<int> > hardClauses;
    std::vector<std::vector<int> > softClauses;
    std::vector<int> solution;
    int top;
};

#endif // __WPMAXSAT_H__
