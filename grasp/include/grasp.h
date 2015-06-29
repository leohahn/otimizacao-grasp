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
    virtual std::vector<bool> makeLocalSearch(std::vector<bool> solution) = 0;
    virtual std::vector<bool> updateSolution(std::vector<bool> imp_solution,
                                             std::vector<bool> best_solution) = 0;

    virtual ~GRASP() {}
};

#endif
