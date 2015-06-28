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
        constructGreddyRandomSolution();
        makeLocalSearch();
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


std::vector<bool> WpMaxSAT::constructGreedyRandomSolution()
{
	std::vector<bool> variableValues(numVariables+1);
	std::vector<bool> satisfiedClausesHard(hardClauses.size());
	std::vector<bool> satisfiedClausesSoft(softClauses.size());
	for(unsigned int i = 0;i<hardClauses.size();i++) {
		satisfiedClausesHard.push_back(false);	  
	}
	for(unsigned int i = 0;i<softClauses.size();i++) {
		satisfiedClausesSoft.push_back(false);	  
	}
	variableValues.push_back(0);  //this position is ignored
	
	
	std::vector< struct candidate > candidates;
	
	for(unsigned int varInx=1;varInx<=numVariables;varInx++) {
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
	std::vector< struct candidate > rcl;
	int sizercl = 40/100.0 * candidates.size();
	
	for(unsigned int i=0)
	

}

void WpMaxSAT::makeLocalSearch()
{

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
{c
    unsigned n_satisf_clauses = 0;
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
