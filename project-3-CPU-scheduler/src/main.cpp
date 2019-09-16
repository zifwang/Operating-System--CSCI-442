#include "simulation.h"
#include "util/flags.h"
#include "util/logger.h"
#include <cstdlib>
#include <string>

using namespace std;


// Entry point to the simulation.
int main(int argc, char** argv) {
  FlagOptions flags = parse_flags(argc, argv);
  Logger logger(flags.verbose, flags.detailed);

  // Create the simulation.
  Simulation simulation(flags.scheduler, logger);

  // Execute the simulation on the provided file.
  simulation.run(flags.filename);

  return EXIT_SUCCESS;
}
