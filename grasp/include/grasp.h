#ifndef __GRASP_H__

//!
//! Classe abstrata GRASP, que contém os métodos necessários para a implementação
//! do algoritmo. A classe é extendida de acordo com o problema escolhido
//!
class GRASP {
public:
    virtual void run(int) = 0;
    virtual void constructGreddyRandomSolution() = 0;
    virtual void makeLocalSearch() = 0;
    virtual void updateSolution() = 0;

    virtual ~GRASP() {}
};

#endif
