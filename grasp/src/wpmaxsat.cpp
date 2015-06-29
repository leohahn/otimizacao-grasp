#include<iostream>
#include "../include/wpmaxsat.h"
#include<vector>
#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <algorithm>
#include <time.h>
#include <assert.h>
#include <limits>
#include <random>
#include <iterator>

using std::vector;

//////////////////
/// Prototypes ///
//////////////////
bool isSolutionStale();
bool iterationsLeft(int, int);
std::vector<std::string> tokenize(std::string line);
std::vector<int> stringVectorToInt(std::vector<std::string> inpt);

template<typename Iter, typename RandomGenerator>
Iter select_randomly(Iter start, Iter end, RandomGenerator& g) {
    std::uniform_int_distribution<> dis(0, std::distance(start, end) - 1);
    std::advance(start, dis(g));
    return start;
}

template<typename Iter>
Iter select_randomly(Iter start, Iter end) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    return select_randomly(start, end, gen);
}

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


struct candidate{
    int variable_index;
    bool value;
    int satisfiedHard;
    int satisfiedSoft;
};

bool candidateSorter(struct candidate const& lhs,struct candidate const& rhs) {
    if(lhs.satisfiedHard!=rhs.satisfiedHard)
        return lhs.satisfiedHard < rhs.satisfiedHard;
    if(lhs.satisfiedSoft!=rhs.satisfiedSoft)
        return lhs.satisfiedSoft < rhs.satisfiedSoft;
    return false;
}

//updates a vector of bools indicating if each clause is satified considering the new variable with a value setup
std::vector<bool> WpMaxSAT::updateClausesSatisfiability(int var, bool var_value, ClauseType type, std::vector<bool> currentSatisfiability)
{
    switch (type) {
    case SOFT:
        for (unsigned i=0; i<softClauses.size(); ++i) {
            if(currentSatisfiability[i]==false){
                int finded_var = findInClause(i, var, SOFT);
                if(var_value) {
                    if (finded_var == var) { // If variable is not negated
                        //satisfies
                        currentSatisfiability[i] = true;
                    }
                } else {
                    if (finded_var == -var) { // If variable is negated
                        currentSatisfiability[i] = true;
                    }
                }
            }
        }
        break;
    case HARD:
        for (unsigned i=0; i<hardClauses.size(); ++i) {
            if(currentSatisfiability[i]==false){
                int finded_var = findInClause(i, var, HARD);
                if(var_value){
                    if (finded_var == var) { // If variable is not negated
                        currentSatisfiability[i] = true;
                    }
                } else {
                    if (finded_var == -var) { // If variable is negated
                        currentSatisfiability[i] = true;
                    }
                }
            }

        }
        break;
    }
    return currentSatisfiability;
}

std::vector<bool> WpMaxSAT::constructGreedyRandomSolution()
{
    std::vector<bool> variableValues(numVariables+1);
    std::vector<bool> variablesUsed(numVariables+1); //indicates if variable has had a value chosen
    std::vector<bool> satisfiedClausesHard(hardClauses.size());
    std::vector<bool> satisfiedClausesSoft(softClauses.size());
    int num_variables_chosen = 0;

    for(unsigned int i = 0;i<hardClauses.size();i++) {
        satisfiedClausesHard.push_back(false);
    }
    for(unsigned int i = 0;i<softClauses.size();i++) {
        satisfiedClausesSoft.push_back(false);
    }
    for(unsigned int i = 0;i<=numVariables;i++) {
        variableValues.push_back(false);
        variablesUsed.push_back(false);
    }

    while(num_variables_chosen < numVariables){
        std::vector< struct candidate > candidates;
        candidates.clear();
        for(unsigned int varInx=1;varInx<=numVariables;varInx++) {
            if(variablesUsed[varInx]==false) {
                struct candidate cand;
                cand.variable_index = varInx;
                //calculates satisfied clauses if = 0
                int satHardFalse = numOfSatisfiedClauses(varInx, false,HARD, satisfiedClausesHard);
                int satSoftFalse = numOfSatisfiedClauses(varInx, false,SOFT, satisfiedClausesSoft);
                //calculates satisfied clauses if = 1
                int satHardTrue = numOfSatisfiedClauses(varInx, false,HARD, satisfiedClausesHard);
                int satSoftTrue = numOfSatisfiedClauses(varInx, false,SOFT, satisfiedClausesSoft);
                if(satHardTrue>satHardFalse){
                    cand.value = true;
                    cand.satisfiedHard = satHardTrue;
                    cand.satisfiedSoft = satSoftTrue;
                } else if(satHardFalse > satHardTrue) {
                    cand.value = false;
                    cand.satisfiedHard = satHardFalse;
                    cand.satisfiedSoft = satSoftFalse;
                } else {
                    if(satSoftTrue>satSoftFalse) {
                        cand.value = true;
                        cand.satisfiedHard = satHardTrue;
                        cand.satisfiedSoft = satSoftTrue;
                    } else {
                        cand.value = false;
                        cand.satisfiedHard = satHardFalse;
                        cand.satisfiedSoft = satSoftFalse;
                    }
                }
            }
        }
        std::vector< struct candidate > rcl;
        int sizercl = floor(rclpercentage/100.0 * candidates.size());
        std::sort(candidates.begin(),candidates.end(), &candidateSorter); //ascending order
        for(unsigned int i=candidates.size()-1;i>=candidates.size()-sizercl;i++){
            rcl.push_back(candidates[i]);
        }
        srand (time(NULL));
        int choice = rand() % sizercl; //chosen which variable will be selected
        int chosenVariable = rcl[choice].variable_index;
        bool chosenVariableValue = rcl[choice].value;
        satisfiedClausesHard = updateClausesSatisfiability(chosenVariable,chosenVariableValue,HARD,satisfiedClausesHard);
        satisfiedClausesSoft = updateClausesSatisfiability(chosenVariable,chosenVariableValue,SOFT,satisfiedClausesSoft);
        variablesUsed[chosenVariable] = true;
        variableValues[chosenVariable] = chosenVariableValue;
        num_variables_chosen++;
    }
    return variableValues;
}

void WpMaxSAT::makeLocalSearch(vector<bool> solution)
{
    const int MAX_STEPS = 200;
    vector<bool> hardScores(hardClauses.size(), 1);
    vector<bool> best_sol = solution;
    int best_gain = std::numeric_limits<int>::min();

    vector<bool> current_sol = solution;
    int current_gain = std::numeric_limits<int>::min();
    for (int i=0; i<MAX_STEPS; ++i) {
        vector<int> hard_decreasing_vars = createHardDecreasingVariables(current_sol);
        vector<int> soft_decreasing_vars = createSoftDecreasingVariables(current_sol);
        int v;

        if (isFeasible(current_sol) && (getSolutionGain(current_sol) > best_gain)) {
            best_sol = current_sol;
            best_gain = current_gain;
        }

        if (!hard_decreasing_vars.empty()) {
            bool all_zeros = true;
            for (unsigned int i=1; i<hard_decreasing_vars.size(); ++i) {
                if (hard_decreasing_vars[i] > 1) {
                    all_zeros = false;
                }
            }
            if (all_zeros == false) {
                do {
                    v = *select_randomly(hard_decreasing_vars.begin(),
                                         hard_decreasing_vars.end());
                } while (v == 0);
            }
        } else if (!soft_decreasing_vars.empty()) {
            // TODO: Select the best (not random)
            bool all_zeros = true;
            for (unsigned int i=1; i<soft_decreasing_vars.size(); ++i) {
                if (hard_decreasing_vars[i] > 1) {
                    all_zeros = false;
                }
            }
            if (all_zeros == false) {
                do {
                    v = *select_randomly(soft_decreasing_vars.begin(),
                                         soft_decreasing_vars.end());
                } while (hard_decreasing_vars[v] > 0);
            } else {
                continue;
            }
        } else {
            continue;// Nao faz nada
        }
        if (current_sol[v] == true) {
            current_sol[v] == false;
        } else {
            current_sol[v] == true;
        }
    }
}

int WpMaxSAT::getSolutionGain(vector<bool> solution)
{
    int gain = 0;
    for (unsigned int i=0; i<softClauses.size(); ++i) {
        for (unsigned int j=1; j<solution.size(); ++j) {
            if (satisfiesClause(j, int(solution[j]), softClauses[i])) {
                gain += softClauses[i][0];
                j = solution.size();
            }
        }
    }
    return gain;
}

vector<int> WpMaxSAT::createHardDecreasingVariables(vector<bool> solution)
{

}

vector<int> WpMaxSAT::createSoftDecreasingVariables(vector<bool> solution)
{
    vector<int> decreasing_variables;

    for (unsigned int i=1; i<solution.size(); ++i) {
        vector<bool> aux_sol = solution;

        if (solution[i] == true) {
            aux_sol[i] = false;
        } else {
            aux_sol[i] = true;
        }

        decreasing_variables.push_back(getSolutionGain(aux_sol));
    }
    return decreasing_variables;
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
}

int WpMaxSAT::getHardScore(int var, int value, const vector<bool>& clauses_val)
{
    int hard_score = 0;
    for (unsigned int i=0; i<clauses_val.size(); ++i) {
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
    for (unsigned int i=0; i<clauses_val.size(); ++i) {
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

int WpMaxSAT::numOfSatisfiedClauses(int var, bool var_value, ClauseType type, std::vector<bool> satisfiedStatus)
{
    unsigned int n_satisf_clauses = 0;
    switch (type) {
    case SOFT:
        for (unsigned i=0; i<softClauses.size(); ++i) {
            if(satisfiedStatus[i]==false){
                int finded_var = findInClause(i, var, SOFT);
                if(var_value) {
                    if (finded_var == var) { // If variable is not negated
                        n_satisf_clauses++;
                    }
                } else {
                    if (finded_var == -var) { // If variable is negated
                        n_satisf_clauses++;
                    }
                }
            }
        }
        break;
    case HARD:
        for (unsigned i=0; i<hardClauses.size(); ++i) {
            if(satisfiedStatus[i]==false){
                int finded_var = findInClause(i, var, HARD);
                if(var_value){
                    if (finded_var == var) { // If variable is not negated
                        n_satisf_clauses++;
                    }
                } else {
                    if (finded_var == -var) { // If variable is negated
                        n_satisf_clauses++;
                    }
                }
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

bool WpMaxSAT::isFeasible(vector<bool> solution)
{
    for (unsigned int i=0; i<hardClauses.size(); ++i) {
        bool last_clause = false;
        for (unsigned int j=1; j<solution.size(); ++j) {
            if (satisfiesClause(j, int(solution[j]), hardClauses[i])) {
                last_clause = true;
                j = solution.size(); // breaks out of loop;
            }
        }
        if (last_clause == false) {
            return false;
        }
    }
    return true;
}
