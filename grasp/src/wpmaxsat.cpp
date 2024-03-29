#include<iostream>
#include "../include/wpmaxsat.h"
#include<vector>
#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <algorithm>
#include <assert.h>
#include <limits>
#include <random>
#include <time.h>
#include <iterator>
#include <omp.h>

using std::vector;
using std::cout;
using std::endl;
//////////////////
/// Prototypes ///
//////////////////
bool isSolutionStale();
bool iterationsLeft(int, int);
std::vector<std::string> tokenize(std::string line);
std::vector<int> stringVectorToInt(std::vector<std::string> inpt);
void printSolution(vector<bool> solution);

WpMaxSAT::WpMaxSAT(std::string inputFile)
{
    parseFile(inputFile);
    //std::cout<<"num clauses"<<hardClauses.size()+softClauses.size()<<"\n";
    //std::cout<<"num variable"<<numVariables<<"\n";
}

WpMaxSAT::~WpMaxSAT()
{

}

void WpMaxSAT::printClause(vector<int> clause)
{
    for (unsigned int i=0; i<clause.size(); ++i) {
        cout << "id: " << i << "= " << clause[i] << endl;
    }
}

void printBoolVector(std::vector<bool>);

void WpMaxSAT::run(int max_iterations)//, String greedySelect)
{
    //cout << "NUM HARD CLAUSES:" << hardClauses.size() << endl;
    int current_iter = 1;

    vector<bool> best_solution;
    while (iterationsLeft(current_iter, max_iterations)) {
        std::cout << "Current iteration " << current_iter << std::endl;
        //
        //sol = GSAT();
        vector<bool> sol;
		int MAX_TRIES=4;
		do{
			sol = constructGreedyRandomSolution();
			MAX_TRIES--;
		} while(!isFeasible(sol) && MAX_TRIES>0);
        std::vector<bool> imp_sol = makeLocalSearch(sol);
        vector<bool> new_sol = updateSolution(imp_sol, best_solution);
        best_solution = new_sol;
		printSolution(best_solution);
        current_iter++;
    }
    cout << "Best solution: " << endl;
    printSolution(best_solution);
    cout << "Feasible: " << isFeasible(best_solution) << endl;
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
std::vector<bool> WpMaxSAT::updateClausesSatisfiability(int var, bool var_value,
                                                        ClauseType type,
                                                        std::vector<bool> currentSatisfiability)
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


std::vector<bool> WpMaxSAT::GSAT()
{
	int walkProb = 20;
	std::vector<bool> solution;
	int MAX_FLIPS = numVariables;
	int numHardClauses = hardClauses.size();
	for(int i=0;i<numVariables+1;i++){
			int tmp = rand() % 2;
			if(tmp==0)
				solution.push_back(false);
			else
				solution.push_back(true);

	}
	if(isFeasible(solution))
	{
	  return solution;
	}
	while(MAX_FLIPS>0) {
		//std::cout<<"entered: flips left"<< MAX_FLIPS<<"\n";
		//initial random solution

		if(rand()%100 < walkProb){ //picks a clause at random. picks one of tha variables within the clause at random
			 int clauseInx = rand() % numHardClauses;
			 int variable = hardClauses[clauseInx][rand() % (hardClauses[clauseInx].size()-1)+1];
			 if(variable<0){
				variable= -variable;
			  }
			 if(solution[variable]==true) {
				solution[variable]=false;
			 } else {
				solution[variable]=true;
			 }

		} else {
			//picks one of the variables with the greatest gain

			//for each variable contains the difference in hard clauses
			//the invertion of its value would cause
			vector<int> gain = createHardDecreasingVariables(solution);
			//gets variables that cause maximum increase
			vector<int> candidates_variables;
			int max_gain = 0;
			for(unsigned int g = 1; g<gain.size();g++){
				if(gain[g] > max_gain){
					candidates_variables.clear();
					candidates_variables.push_back(g);
					max_gain = gain[g];
				} else if(gain[g]==max_gain){
					candidates_variables.push_back(g);
				}
			}
			//selects one of the top variables randomly
			int var_index = candidates_variables[rand() % candidates_variables.size()];
			if(solution[var_index]==true){
				solution[var_index]=false;
			} else {
				solution[var_index]=true;
			}
		}
		MAX_FLIPS--;

		if(isFeasible(solution))
		{
			return solution;
		}
	}
}

std::vector<bool> WpMaxSAT::constructGreedyRandomSolution()
{
    std::vector<bool> variableValues(numVariables+1);
    std::vector<bool> variablesUsed(numVariables+1); //indicates if variable has had a value chosen
    std::vector<bool> satisfiedClausesHard(hardClauses.size());
    std::vector<bool> satisfiedClausesSoft(softClauses.size());
    int num_variables_chosen = 0;

    for(unsigned int i = 0;i<hardClauses.size();i++) {
        satisfiedClausesHard[i] = false;
    }
    for(unsigned int i = 0;i<softClauses.size();i++) {
        satisfiedClausesSoft[i] = false;
    }
    for(int i = 0;i<=numVariables;i++) {
        variableValues[i]= false;
        variablesUsed[i]= false;
    }
    while(num_variables_chosen < numVariables){
        std::vector< struct candidate > candidates;
        candidates.clear();
        for(int varInx=1;varInx<=numVariables;varInx++) {
            if(variablesUsed[varInx]==false) {
                struct candidate cand;
                cand.variable_index = varInx;
                //calculates satisfied clauses if = 0
                int satHardFalse = numOfSatisfiedClauses(varInx, false,HARD, satisfiedClausesHard);
                int satSoftFalse = numOfSatisfiedClauses(varInx, false,SOFT, satisfiedClausesSoft);
                //calculates satisfied clauses if = 1
                int satHardTrue = numOfSatisfiedClauses(varInx, true,HARD, satisfiedClausesHard);
                int satSoftTrue = numOfSatisfiedClauses(varInx, true,SOFT, satisfiedClausesSoft);
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
                candidates.push_back(cand);
            }
        }
        std::vector< struct candidate > rcl;
        int sizercl = floor(rclpercentage/100.0 * candidates.size());
        std::sort(candidates.begin(),candidates.end(), &candidateSorter); //ascending order
        for(unsigned int i=candidates.size()-1;i>=candidates.size()-sizercl;i--){
			//std::cout<<"rcl element:"<<candidates[i].satisfiedHard<<"\n";
            rcl.push_back(candidates[i]);
        }

        int candidateIndex = rand() % candidates.size();
        //cout << "Candidate Chosen: " << candidateIndex << endl;
        //candidate choice = *select_randomly(rcl.begin(),rcl.end());//chosen which variable will be selected
        candidate choice = candidates[candidateIndex];

        int chosenVariable = choice.variable_index;
        bool chosenVariableValue = choice.value;
        satisfiedClausesHard = updateClausesSatisfiability(chosenVariable,chosenVariableValue,HARD,satisfiedClausesHard);
        satisfiedClausesSoft = updateClausesSatisfiability(chosenVariable,chosenVariableValue,SOFT,satisfiedClausesSoft);
        //cout << "Satisfied clauses" << endl;
        //cout << "Hard" << endl;
        //printBoolVector(satisfiedClausesHard);

        //cout << "\n";
//cout << "Soft" << endl;
        //printBoolVector(satisfiedClausesSoft);
        variablesUsed[chosenVariable] = true;

        variableValues[chosenVariable] = chosenVariableValue;

        num_variables_chosen++;

    }

    //cout << "SATISFIED HARD CLAUSES" << endl;
    //printBoolVector(satisfiedClausesHard);
    //cout << "COMECANDO A PRINTAR ISFEASIBLE" << endl;
    //cout << isFeasible(variableValues) << endl;

    return variableValues;
}

vector<bool> WpMaxSAT::makeLocalSearch(vector<bool> solution)
{
    //cout << "Beginning search solution" << endl;
    //printSolution(solution);
    const int MAX_STEPS = 30;

    vector<bool> hardScores(hardClauses.size(), 1);
    vector<bool> best_sol = solution;
    int best_gain = std::numeric_limits<int>::min();

    vector<bool> current_sol = solution;
    int current_gain = std::numeric_limits<int>::min();
    for (int s=0; s<MAX_STEPS; ++s) {
        //std::cout<<"local search step:"<<i<<"\n";
        //std::cout<<"begin hard\n";

        vector<int> hard_decreasing_vars = createHardDecreasingVariables(current_sol);
        int val_index;
        int value;
        //printIntVector(hard_decreasing_vars);
        //std::cout<<"end hard\n";
        //std::cout<<"begin soft\n";
        vector<int> soft_decreasing_vars = createSoftDecreasingVariables(current_sol);

        //std::cout<<"end soft\n";
        if (isFeasible(current_sol)){
            int v = getSolutionGain(current_sol);

            if (v > best_gain) {
                best_sol = current_sol;
                best_gain = v;
                current_gain = v;
                //cout << "________________________________ACHEI UMA FEASIBLE_________________________________" << endl;
            }
        }

        bool less_or_zeros = true;

        if (hardClauses.size() != 0) {
            for (unsigned int i=1; i<hard_decreasing_vars.size(); ++i) {
                if (hard_decreasing_vars[i] > 0) {
                    less_or_zeros = false;
                }
            }
        }
        //printIntVector(hard_decreasing_vars);
        if (less_or_zeros == false) {
            do {
                val_index = (rand() % (hard_decreasing_vars.size() - 1)) + 1;
            } while (hard_decreasing_vars[val_index] <= 0);
            value = hard_decreasing_vars[val_index];

            if (current_sol[val_index] == true) {
                current_sol[val_index] = false;
            } else {
                current_sol[val_index] = true;
            }
        } else {
            less_or_zeros = true;
            for (unsigned int i=1; i<soft_decreasing_vars.size(); ++i) {
                if (soft_decreasing_vars[i] > 0) {
                    less_or_zeros = false;
                }
            }

            if (less_or_zeros == false) {
                val_index = (rand() % (soft_decreasing_vars.size() - 1)) + 1;
                if (hard_decreasing_vars.size() != 0) {
                    while (hard_decreasing_vars[val_index] > 0) {
                        val_index = (rand() % (soft_decreasing_vars.size() - 1)) + 1;
                    }
                }
                //printIntVector(soft_decreasing_vars);
                int best_index = 1;
                for (int i=2; i<soft_decreasing_vars.size(); ++i) {
                    if (soft_decreasing_vars[i] > soft_decreasing_vars[best_index] && hard_decreasing_vars[i] == 0) {
                        best_index = i;
                    }
                }

                if (hard_decreasing_vars.size() == 0 || hard_decreasing_vars[best_index] == 0) {
                    //printIntVector(soft_decreasing_vars);
                    value = soft_decreasing_vars[val_index];
                    if (current_sol[best_index] == true) {
                        current_sol[best_index] = false;
                    } else {
                        current_sol[best_index] = true;
                    }
                } else {
                    s = MAX_STEPS;
                    cout << "DEU STALL" << endl;
                }
            } else {
                s = MAX_STEPS;
                cout << "DEU STALL" << endl;
            }
        }
    }
    //printSolution(best_sol);
    return best_sol;
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

int WpMaxSAT::getSolutionGainHard(vector<bool> solution){
    int gain = 0;
    for (unsigned int i=0; i<hardClauses.size(); ++i) {
        for (unsigned int j=1; j<solution.size(); ++j) {
            if (satisfiesClause(j, int(solution[j]), hardClauses[i])) {
                gain += 1;//hardClauses[i][0];
                j = solution.size();
            }
        }
    }
    return gain;
}

vector<int> WpMaxSAT::createHardDecreasingVariables(vector<bool> solution)
{
    std::vector<int> results(solution.size(), 0);
    int baseline = getSolutionGainHard(solution);
    for (unsigned i=1; i<solution.size(); ++i) {
        int temp;
        vector<bool> newSol(solution);
        if(newSol[i])
            newSol[i]=false;
        else
            newSol[i]=true;
        temp = getSolutionGainHard(newSol)-baseline;

        results[i] = temp;

    }

    return results;
}

vector<int> WpMaxSAT::createSoftDecreasingVariables(vector<bool> solution)
{
    vector<int> decreasing_variables(solution.size(),0);
    int gain_solution = getSolutionGain(solution);
#pragma omp parallel for shared( decreasing_variables)
    for (unsigned int i=1; i<solution.size(); ++i) {
        vector<bool> aux_sol = solution;

        if (solution[i] == true) {
            aux_sol[i] = false;
        } else {
            aux_sol[i] = true;
        }
        int result = getSolutionGain(aux_sol) - gain_solution;
        if (result < 0) {
            result = 0;
        }
        decreasing_variables[i]= result;
    }
    return decreasing_variables;
}

bool WpMaxSAT::satisfiesClause(int var, int value, vector<int> clause)
{
    for(unsigned int i = 1; i<clause.size();i++) {
        if(clause[i]==var) {
            if(value==true){
                return true;
            }
        } else if(-clause[i] == var) {
            if(value==false){
                return true;
            }
        }
    }
    return false;
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

vector<bool> WpMaxSAT::updateSolution(vector<bool> imp_solution,
                                      vector<bool> best_solution)
{
    bool imp_feasible = isFeasible(imp_solution);
    bool best_feasible = isFeasible(best_solution);

    if (imp_feasible == true) {
        if (best_feasible == false) {
            cout << "Better Solution Found:" << endl;
            printSolution(imp_solution);
            return imp_solution;
        }
        if (getSolutionGain(imp_solution) > getSolutionGain(best_solution)) {
            return imp_solution;
        }
        return best_solution;
    } else {
        return best_solution;
    }
}

void WpMaxSAT::parseFile(std::string path)
{
    std::vector<std::string> lines;
    std::string line;
    int numVar = 0;
    std::ifstream myfile (path);
	int top = 0;
    //parses file
	bool has_top = false;
    if (myfile.is_open()) {
        while ( getline(myfile,line) ) {
            if(line[0]=='p'){
                std::vector<std::string> columns = tokenize(line);
                numVar = std::stoi(columns[2]);
				if(columns.size()==5)
				{
					has_top=true;
					top = std::stoi(columns[4]);
				}
            }
            else if(line[0]!='c') {
                //removendo caractere de terminação
                line.erase(line.end()-1);
                lines.push_back(line);
            }
        }
        myfile.close();
    }

    std::vector< std::vector<int> > allClauses;
    for(unsigned int i=0;i<lines.size();i++) {
        //get values in the line
        // first element is weight
        //both hard and soft clauses have height
        std::vector<int> clause = stringVectorToInt(tokenize(lines[i]));
        allClauses.push_back(clause);
    }
    //puts each clause in its respective list
    for(unsigned int i=0;i<allClauses.size();i++) {
        if(allClauses[i][0]<top || !has_top) { //soft
            softClauses.push_back(allClauses[i]);
        } else {
            hardClauses.push_back(allClauses[i]);
        }
    }
    //std::cout<<"loaded soft clauses:"<<softClauses.size()<<"\n";
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

// Function that finds a variable in a clause. Returns the value of the variable (variable or -variable).
// If nothing was found, returns 0.
int WpMaxSAT::findInClause(int clause, int var, ClauseType type)
{

	std::vector<int> clauseVec;
	switch(type){
	  case SOFT:
		clauseVec = softClauses[clause];
		break;
	  case HARD:
		clauseVec = hardClauses[clause];
	}

	for(unsigned int i=1;i<clauseVec.size();i++) {
		if(clauseVec[i]==var || clauseVec[i]==-var){
			return clauseVec[i];
		}
	}
	return 0;
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
    int aux;
    for (unsigned int i=0; i<hardClauses.size(); ++i) {
        bool last_clause = false;
        for (unsigned int j=1; j<solution.size(); ++j) {
            if (satisfiesClause(j, int(solution[j]), hardClauses[i])) {
                last_clause = true;
                aux = j;
                j = solution.size(); // breaks out of loop;
            }
        }
        if (last_clause == false) {
            //cout << "isFeasible FALHOU: " << endl;
            //printIntVector(hardClauses[i]);
            //cout << "index: " << aux <<", ";
            //cout << "value: " << solution[aux] << endl;
            return false;
        }
    }
    return true;
}

void WpMaxSAT::printSolution(vector<bool> solution)
{
    std::cout << "Solution: soft: " <<  getSolutionGain(solution)<<" hard: "<<getSolutionGainHard(solution) << std::endl;
}

void WpMaxSAT::printIntVector(std::vector<int> vec) {
    for(unsigned int i=0;i<vec.size();i++) {
        std::cout<<vec[i]<<" ";
    }
    std::cout<<"\n";
}

void printBoolVector(std::vector<bool> vec) {
    for(unsigned int i=0;i<vec.size();i++) {
        std::cout<<vec[i]<<" ";
    }
    std::cout<<"\n";
}
