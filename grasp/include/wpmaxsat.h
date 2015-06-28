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
    void constructGreedyRandomSolution();
    void makeLocalSearch();
    void updateSolution();

    
    enum ClauseType {SOFT, HARD};

private:

    int numOfSatisfiedClauses(int var, int var_value, ClauseType type);
    int getNumVariables();
    int findInClause(int clause, int var, ClauseType type);
    void parseFile(std::string path);
    int numVariables;
    std::vector<std::vector<int> > hardClauses;
    std::vector<std::vector<int> > softClauses;
    int top;
};

#endif // __WPMAXSAT_H__
