#include <iostream>
#include <string>
#include "../include/wpmaxsat.h"

void printIntVector(std::vector<int> vec){
	for(unsigned int i=0;i<vec.size();i++) {
		std::cout<<vec[i]<<" ";
	}
	std::cout<<"\n";
}

void run(std::string name){

    std::cout<<"START!"<<name;
    WpMaxSAT wp= WpMaxSAT(name);
    wp.run(80);
}


int main(int argc, char **argv)
{
    run("../../instances/original/hamming10-2.clq.wcnf");
    //run("../../instances/original/MANN_a45.clq.wcnf");
    //run("../../instances/original/p_hat1000-3.clq.wcnf");
    //run("../../instances/original/hamming8-2.clq.wcnf");
    //run("../../instances/original/p_hat1000-1.clq.wcnf");
    //run("../../instances/original/johnson8-4-4.clq.wcnf");
    //run("../../instances/original/p_hat1000-2.clq.wcnf");
}
