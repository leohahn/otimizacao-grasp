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
      WpMaxSAT wp= WpMaxSAT("../../instances/original/hamming10-2.clq.wcnf");
	  wp.run(1);

}
