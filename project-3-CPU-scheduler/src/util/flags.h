#include <string>
#include "algorithms/scheduler.h"


struct FlagOptions {
  std::string filename;
  bool verbose;
  bool detailed;
  Scheduler* scheduler = nullptr;
};


/**
 * Prints information about how to use this program.
 */
void print_usage();


/**
 * Parses any provided flags, populating the given config and file path as
 * appropriate.
 */
FlagOptions parse_flags(int argc, char** argv);


/**
 * Returns a new instance of a scheduler, as specified by the flags.
 */
Scheduler* instantiate_scheduler();
