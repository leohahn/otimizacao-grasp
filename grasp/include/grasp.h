#ifndef __GRASP_H__
#include <vector>
//!
//! Classe abstrata GRASP, que contém os métodos necessários para a implementação
//! do algoritmo. A classe é extendida de acordo com o problema escolhido
//!
class GRASP {
public:
    virtual void run(int max_iterations) = 0;
    virtual std::vector<bool> constructGreedyRandomSolution() = 0;
    virtual void makeLocalSearch(std::vector<bool> sol) = 0;
    virtual void updateSolution() = 0;

    virtual ~GRASP() {}
};

#endif
