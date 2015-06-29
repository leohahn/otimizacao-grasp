#include <iostream>
#include "../include/wpmaxsat.h"

void printIntVector(std::vector<int> vec){
	for(unsigned int i=0;i<vec.size();i++) {
		std::cout<<vec[i]<<" ";
	}
	std::cout<<"\n";
}

int main(int argc, char **argv)
{

<<<<<<< HEAD
    WpMaxSAT wp= WpMaxSAT("../../instances/original/test2.wcnf");
    wp.run(1);
=======
    WpMaxSAT wp= WpMaxSAT("../../instances/original/p_hat1000-1.clq.wcnf");
    wp.run(10);
>>>>>>> b6204bc0e3e30388a9572be5cf83395e7612c1a2


}
