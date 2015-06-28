#ifndef __WPMAXSAT_H__

#include "grasp.h"
#include <vector>
#include <string>
class WpMaxSAT : public GRASP {
public:
    WpMaxSAT();
    ~WpMaxSAT();
    void run();
    void constructGreddyRandomSolution();
    void makeLocalSearch();
    void updateSolution();
    void parseFile(std::string path);
   
private:
    std::vector<int> rcl;
    
    //variable values start from index 1
    //index 0 is ignored
    std::vector<int> variables;
    int numVariables;
    std::vector<std::vector<int> > hardClauses;
    std::vector<std::vector<int> > softClauses;

    std::vector<int> solution;
    int top;
};
#endif
