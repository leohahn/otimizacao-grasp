#include<iostream>
#include "../include/wpmaxsat.h";
#include<vector>
#include <iostream>
#include <fstream>
#include <string>


std::vector<int> tokenizeInts(std::string line){
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
	std::vector<int> nums;
	
	for(unsigned int i = 0; i<values.size();i++) {
		int num = std::stoi(values[i]);
		nums.push_back(num);
		std::cout << num<<" ";
	}
	return nums;	
}


WpMaxSAT::WpMaxSAT()
{
}

void WpMaxSAT::run()
{

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

WpMaxSAT::~WpMaxSAT()
{

}


void WpMaxSAT::parseFile(std::string path)
{
	std::vector<std::string> lines;
	std::string line;
	int top = 0;
	std::ifstream myfile (path);
	//parses file
	if (myfile.is_open()) {
		while ( getline(myfile,line) ) {
			if(line[0]!='c' && line[0]!='p') {
				//removendo caractere de terminação
				line.erase(line.end()-1);
				lines.push_back(line);
				if(int(line[0]) > top)
					top = int(line[0]);
			}
		}
	  myfile.close();
	}
	
	for(unsigned int i=0;i<lines.size();i++) {
		tokenizeInts(lines[i]);

	}

}




