#include "../include/wpmaxsat.h"

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

int WpMaxSAT::numOfSatisfiedClauses(int var) const
{
    return -1;
}

int WpMaxSAT::getNumVariables() const
{
    return -1;
}

bool iterationsLeft(int current_iter, int max_iterations)
{
    return current_iter < max_iterations;
}

bool isSolutionStale()
{
    return true;
}
