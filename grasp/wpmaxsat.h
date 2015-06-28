#ifndef __WPMAXSAT_H__

#include "grasp.h"
#include <vector>

class WpMaxSAT : public GRASP {
public:
    WpMaxSAT();
    ~WpMaxSAT();
    void run();
    void constructGreddyRandomSolution();
    void makeLocalSearch();
    void updateSolution();
    void numOfSatisfiedClauses() const;
private:
    std::vector<int> rcl;
    std::vector<int> variables;
    std::vector<std::vector<int> > hardClauses;
    std::vector<std::vector<int> > softClauses;
    std::vector<int> solution;
    int top;
};
#endif
