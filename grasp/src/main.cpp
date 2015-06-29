#include <iostream>
#include "../include/wpmaxsat.h"

void printIntVector(std::vector<int> vec){
	for(unsigned int i=0;i<vec.size();i++) {
		std::cout<<vec[i]<<" ";
	}
	std::cout<<"\n";
}


void run(String name){
	
  	std::cout<<"START!"<<name;
	WpMaxSAT wp= WpMaxSAT(name);
    wp.run(80);
}


int main(int argc, char **argv)
{
	run("hamming10-2.clq.wcnf");
	run("MANN_a45.clq.wcnf");
	run("p_hat1000-3.clq.wcnf");
	run("hamming8-2.clq.wcnf");
	run("p_hat1000-1.clq.wcnf");
	run("johnson8-4-4.clq.wcnf");
	run("p_hat1000-2.clq.wcnf");
}

     
      
  
