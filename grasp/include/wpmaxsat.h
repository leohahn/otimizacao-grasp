#ifndef __WPMAXSAT_H__

#include "grasp.h"
#include <vector>

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
private:
    int numOfSatisfiedClauses(int var) const;
    int getNumVariables() const;

    std::vector<int> rcl_;
    std::vector<int> variables_;
    std::vector<std::vector<int> > hardClauses_;
    std::vector<std::vector<int> > softClauses_;
    std::vector<int> solution_;
    int top;
};

#endif // __WPMAXSAT_H__
