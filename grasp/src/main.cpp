#include <iostream>
#include "../include/wpmaxsat.h"
int main(int argc, char **argv) {
      WpMaxSAT wp= WpMaxSAT();
      wp.parseFile("../../instances/original/hamming10-2.clq.wcnf");
}

