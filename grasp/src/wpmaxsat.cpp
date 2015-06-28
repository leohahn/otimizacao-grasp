#include<iostream>
#include "../include/wpmaxsat.h";
#include<vector>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

using std::vector;

std::vector<std::string> tokenize(std::string line){
	std::vector<std::string> values;
	std::string valueTemp="";
 	//std::cout<< line<<"\n";
	
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








//////////////////
/// Prototipes ///
//////////////////
bool isSolutionStale();
bool iterationsLeft(int, int);

WpMaxSAT::WpMaxSAT()
{

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


void WpMaxSAT::constructGreddyRandomSolution()
{

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
	for(int i = 0;i<=numVar;i++){
		variables.push_back(0);
	}
	numVariables = numVar;
}




int WpMaxSAT::numOfSatisfiedClauses(int var, int var_value, ClauseType type)
{
    unsigned n_satisf_clauses = 0;
    switch (type) {
    case SOFT:
        for (unsigned i=0; i<softClauses.size(); ++i) {
            if (findInClause(i, var, SOFT) != -1) {
                n_satisf_clauses++;
            }
        }
        break;
    case HARD:
        break;
    }

    return -1;
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
    return true;
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
>>>>>>> fccecd545f357d29e704faf7e8763749ab6cb54e
