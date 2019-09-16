/**
 * This file contains the main() function that drives the simulation. You'll
 * need to add logic to this file to create a Simulation instance and invoke its
 * run() method.
 */

#include <cstdlib>
#include <iostream>
#include "simulation/simulation.h"
#include "flag_parser/flag_parser.h"
using namespace std;


/**
 * The main entry point to the simulation.
 */
int main(int argc, char** argv) {
  FlagOptions opt;

  if (!parse_flags(argc,argv,opt)){
    print_usage();
    return EXIT_FAILURE;
  }

  Simulation sim(opt);
  sim.run();

  return EXIT_SUCCESS;
}
