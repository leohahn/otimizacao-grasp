#ifndef __WPMAXSAT_H__

#include "grasp.h"
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
    std::vector<bool> constructGreedyRandomSolution();
    std::vector<bool> makeLocalSearch(std::vector<bool> solution);
    std::vector<bool> updateSolution(std::vector<bool> imp_solution,
                                     std::vector<bool> best_solution);

    enum ClauseType {SOFT, HARD};

private:
    int numOfSatisfiedClauses(int var, bool var_value,
                              ClauseType type, std::vector<bool> satisfiedStatus);
    int getNumVariables();
    int findInClause(int clause, int var, ClauseType type);
    void parseFile(std::string path);
    int getHardScore(int var, int value, const std::vector<bool>& clauses_val);
    int getSoftScore(int var, int value, const std::vector<bool>& clauses_val);
    bool satisfiesClause(int var, int value, std::vector<int> clause);

    std::vector<int> createHardDecreasingVariables(std::vector<bool> solution);
    std::vector<int> createSoftDecreasingVariables(std::vector<bool> solution);
    std::vector<bool> createSoftClausesSolution(std::vector<bool> solution);
    std::vector<bool> createHardClausesSolution(std::vector<bool> solution);
    bool isFeasible(std::vector<bool> solution);
    int getSolutionGain(std::vector<bool> solution);
    std::vector<bool> updateClausesSatisfiability(int var, bool var_value,
                                                  ClauseType type,
                                                  std::vector<bool> currentSatisfiability);

    int numVariables;
    std::vector<std::vector<int> > hardClauses;
    std::vector<std::vector<int> > softClauses;
    int top;
    const int rclpercentage = 40;
};

#endif // __WPMAXSAT_H__
