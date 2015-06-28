#include "../include/wpmaxsat.h"
#include <algorithm>
#include <assert.h>
using std::vector;

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
