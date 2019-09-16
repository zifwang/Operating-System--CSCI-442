#include "flags.h"
#include "algorithms/fcfs_scheduler.h"
#include "algorithms/multilevel_feedback_scheduler.h"
#include "algorithms/priority_scheduler.h"
#include "algorithms/round_robin_scheduler.h"
#include <iostream>
#include <cstdlib>
#include <getopt.h>
#include <fstream>

using namespace std;


void print_usage() {
  cout <<
      "Usage: sim [-dvh] filename\n"
      "\n"
      "Options:\n"
      "  -h, --help:\n"
      "      Print this help message and exit.\n"
      "  -t, --per_thread:\n"
      "      If set, outputs per-thread metrics at the end of the simulation.\n"
      "  -v, --verbose:\n"
      "      If set, outputs all state transitions and scheduling choices.\n"
      "  -a, --algorithm <algorithm>:\n"
      "      The scheduling algorithm to use. Valid values are:\n"
      "        FCFS: first-come, first-served (default)\n"
      "        RR: round-robin scheduling\n"
      "        PRIORITY: priority scheduling\n"
      "        MLFQ: multilevel feedback queue\n";
}


FlagOptions parse_flags(int argc, char** argv) {
  FlagOptions flags;

  // Command-line flags accepted by this program.
  static struct option flag_options[] = {
    {"per_thread", no_argument,       0, 't'},
    {"verbose",    no_argument,       0, 'v'},
    {"algorithm",  required_argument, 0, 'a'},
    {"help",       no_argument,       0, 'h'},
    {0, 0, 0, 0}
  };

  int option_index;
  char flag_char;

  // Parse flags entered by the user.
  while (true) {
    flag_char = getopt_long(argc, argv, "-tvha:", flag_options, &option_index);

    // Detect the end of the options.
    if (flag_char == -1) {
      break;
    }

    switch (flag_char) {
      case 't':
        flags.detailed = true;
        break;

      case 'v':
        flags.verbose = true;
        break;

      case 'a':
        flags.scheduler = instantiate_scheduler();
        break;

      case 'h':
        print_usage();
        exit(EXIT_SUCCESS);
        break;

      case 1:
        flags.filename = optarg;
        break;

      default:
        print_usage();
        exit(EXIT_FAILURE);
    }
  }

  if (flags.filename == "") {
    print_usage();
    exit(EXIT_FAILURE);
  }

  if (flags.scheduler == nullptr) {
    flags.scheduler = new FcfsScheduler();
  }

  return flags;
}


Scheduler* instantiate_scheduler() {
  Scheduler* scheduler;
  string option(optarg);

  if (option == "FCFS") {
    scheduler = new FcfsScheduler();
  } else if (option == "RR") {
    scheduler = new RoundRobinScheduler(3);
  } else if (option == "PRIORITY") {
    scheduler = new PriorityScheduler();
  } else if (option == "MLFQ") {
    scheduler = new MultilevelFeedbackScheduler();
  } else {
    print_usage();
    exit(EXIT_FAILURE);
  }

  return scheduler;
}
