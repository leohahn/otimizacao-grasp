#include<iostream>
#include "../include/wpmaxsat.h"
#include<vector>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <assert.h>

using std::vector;

//////////////////
/// Prototypes ///
//////////////////
bool isSolutionStale();
bool iterationsLeft(int, int);
std::vector<std::string> tokenize(std::string line);
std::vector<int> stringVectorToInt(std::vector<std::string> inpt);

WpMaxSAT::WpMaxSAT(std::string inputFile)
{
    parseFile(inputFile);
}

WpMaxSAT::~WpMaxSAT()
{

}

void WpMaxSAT::run(int max_iterations)
{
    int current_iter = 1;
    while (iterationsLeft(current_iter, max_iterations) && !isSolutionStale()) {
        vector<bool> sol = constructGreedyRandomSolution();
        makeLocalSearch(sol);
        updateSolution();
    }
}


std::vector<bool> WpMaxSAT::constructGreedyRandomSolution()
{
    std::vector<bool> variableValues(numVariables+1);
    variableValues.push_back(0);  //this position is ignored
    for(int varInx=1; varInx<=numVariables; varInx++) {
        variableValues.push_back(0);
    }
}

void WpMaxSAT::makeLocalSearch(vector<bool> sol)
{
    int max_steps = 200;
    vector<bool> hardScores(hardClauses.size(), 1);
    vector<int> hard_decreasing_vars = createHardDecreasingVariables();
    vector<int> soft_decreasing_vars = createSoftDecreasingVariables();

    for (int i=0; i<200; ++i) {

    }
}

vector<int> WpMaxSAT::createHardDecreasingVariables()
{

}

vector<int> WpMaxSAT::createSoftDecreasingVariables()
{

}

bool WpMaxSAT::satisfiesClause(int var, int value, vector<int> clause)
{
    vector<int>::iterator it = std::find(clause.begin()+1, clause.end(), var);
    if (it == clause.end()) {
        return false;
    }
    if (*it < 0 && value == 0) {
        return true;
    }
    if (value == 1) {
        if (*it > 0) {
            return true;
        }
    }

    return false;
}

int WpMaxSAT::getHardScore(int var, int value, const vector<bool>& clauses_val)
{
    int hard_score = 0;
    for (int i=0; i<clauses_val.size(); ++i) {
        if (clauses_val[i] == false) {
            if (satisfiesClause(var, value, hardClauses[i]) == true) {
                hard_score += hardClauses[i][0];
            }
        }
    }
    return hard_score;
}

int WpMaxSAT::getSoftScore(int var, int value, const vector<bool>& clauses_val)
{
    int soft_score = 0;
    for (int i=0; i<clauses_val.size(); ++i) {
        if (clauses_val[i] == false) {
            if (satisfiesClause(var, value, softClauses[i]) == true) {
                soft_score += softClauses[i][0];
            }
        }
    }
    return soft_score;
}

void WpMaxSAT::updateSolution()
{

}

void WpMaxSAT::parseFile(std::string path)
{
    std::vector<std::string> lines;
    std::string line;
    int numVar = 0;
    std::ifstream myfile (path);
    //parses file
    if (myfile.is_open()) {
        while ( getline(myfile,line) ) {
            if(line[0]=='p'){
                std::vector<std::string> columns = tokenize(line);
                numVar = std::stoi(columns[2]);
            }
            else if(line[0]!='c') {
                //removendo caractere de terminação
                line.erase(line.end()-1);
                lines.push_back(line);
            }
        }
        myfile.close();
    }
    int top = 0;
    std::vector< std::vector<int> > allClauses;
    for(unsigned int i=0;i<lines.size();i++) {
        //get values in the line
        // first element is weight
        //both hard and soft clauses have height
        std::vector<int> clause = stringVectorToInt(tokenize(lines[i]));
        allClauses.push_back(clause);
        if(clause[0]>top)
            top = clause[0];
    }
    //puts each clause in its respective list
    for(unsigned int i=0;i<allClauses.size();i++) {
        if(allClauses[i][0]<top) { //soft
            softClauses.push_back(allClauses[i]);
        } else {
            hardClauses.push_back(allClauses[i]);
        }
    }
    numVariables = numVar;
}

int WpMaxSAT::numOfSatisfiedClauses(int var, int var_value, ClauseType type)
{
    unsigned n_satisf_clauses = 0;
    switch (type) {
    case SOFT:
        for (unsigned i=0; i<softClauses.size(); ++i) {
            int finded_var = findInClause(i, var, SOFT);
            switch (var_value) {
            case 1:
                if (finded_var == var) { // If variable is not negated
                    n_satisf_clauses++;
                }
                break;
            case 0:
                if (finded_var == -var) { // If variable is negated
                    n_satisf_clauses++;
                }
                break;
            default:
                assert(false);
                break;
            }
        }
        break;
    case HARD:
        for (unsigned i=0; i<hardClauses.size(); ++i) {
            int finded_var = findInClause(i, var, HARD);
            switch (var_value) {
            case 1:
                if (finded_var == var) { // If variable is not negated
                    n_satisf_clauses++;
                }
                break;
            case 0:
                if (finded_var == -var) { // If variable is negated
                    n_satisf_clauses++;
                }
                break;
            default:
                assert(false);
                break;
            }
        }
        break;
    }

    return n_satisf_clauses;
}

int WpMaxSAT::getNumVariables()
{
    // TODO: Implementation
    return -1;
}

bool iterationsLeft(int current_iter, int max_iterations)
{
    return current_iter <= max_iterations;
}

bool isSolutionStale()
{
    // TODO: Implementation
    return false;
}

// Function that finds a variable in a clause. Returns the value of the variable.
// If nothing was found, returns -1.
int WpMaxSAT::findInClause(int clause, int var, ClauseType type)
{
    vector<int>::iterator it;
    int var_value = 0;
    switch (type) {
    case SOFT:
        it = std::find(softClauses.at(clause).begin(),
                       softClauses.at(clause).end(), var);

        if (it == softClauses.at(clause).end()) {
            return -1; // Not found
        } else {
            var_value = *it;
        }
        break;

    case HARD:
        it = std::find(hardClauses.at(clause).begin(),
                       hardClauses.at(clause).end(), var);

        if (it == hardClauses.at(clause).end()) {
            return -1;
        } else {
            var_value = *it;
        }
        break;
    }

    return var_value;
}

std::vector<std::string> tokenize(std::string line)
{
    std::vector<std::string> values;
    std::string valueTemp="";

    for(unsigned int j=0;j<line.size();j++) {
        if(line[j]==' '){
            if(valueTemp.size()!=0) {
                values.push_back(valueTemp);
            }
            valueTemp="";
        } else {
            valueTemp += line[j];
        }
    }
    if(valueTemp!="")
        values.push_back(valueTemp);
    return values;
}

std::vector<int> stringVectorToInt(std::vector<std::string> inpt)
{
    std::vector<int> nums;

    for(unsigned int i = 0; i<inpt.size();i++) {
        int num = std::stoi(inpt[i]);
        nums.push_back(num);
    }
    return nums;
}
