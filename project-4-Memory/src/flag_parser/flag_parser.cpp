/**
 * This file contains implementations for methods in the flag_parser.h file.
 *
 * You'll need to add code here to make the corresponding tests pass.
 */

#include "flag_parser/flag_parser.h"
#include <iostream>
#include <getopt.h>
#include <stdio.h>
#include <string.h>

using namespace std;


bool is_valid_frame(char *c, int &frame_max) {
  bool result = true;

  if (isdigit(c[0])){
    frame_max = atoi(c);
    if (!frame_max) {
      result = false;
    }
  }else {
    result = false;
  }

  return result;
}

// Help function printing
void print_usage() {
  cout <<
      "Usage: mem-sim [options] filename\n"
      "\n"
      "Options:\n"
      "  -v, --verbose\n"
      "      Output information about every memory access.\n"
      "\n"
      "  -s, --strategy (FIFO | LRU)\n"
      "      The replacement strategy to use. One of FIFO or LRU.\n"
      "\n"
      "  -f, --max-frames <positive integer>\n"
      "      The maximum number of frames a process may be allocated.\n"
      "\n"
      "  -h --help\n"
      "      Display a help message about these flags and exit\n"
      "\n";
}

// Parses any provided flags
bool parse_flags(int argc, char** argv, FlagOptions &flags) {
  // Command-line flags accpeted by this program
  // long format
  const struct option flag_options[] = {
    {"verbose",    no_argument,       0, 'v'},
    {"strategy",   required_argument, 0, 's'},
    {"max-frames", required_argument, 0, 'f'},
    {"help",       no_argument,       0, 'h'},
    {NULL,0,NULL,0}
  };

  int option_index;
  int error = 0; 
  char flag_char;

  // Parse flags entered by the user
  while(true) {
    flag_char = getopt_long(argc,argv,"vhs:f:",flag_options,&option_index);

    // Detect the end of the options.
    if (flag_char == -1) {
      break;
    }

    switch(flag_char) {
      case 'v':
        flags.verbose = true;
        break;

      case 's':
        if (!strcmp(optarg,"FIFO")) {
          flags.strategy = ReplacementStrategy::FIFO;
        } 
        else if (!strcmp(optarg,"LRU")) {
          flags.strategy = ReplacementStrategy::LRU;
        }
        else {
          error ++;
          return false;
        }
        break;

      case 'f':
        if(!is_valid_frame(optarg,flags.max_frames)){
          return false;
        }
        break;

      case 'h':
        print_usage();
        exit(EXIT_SUCCESS);

      case '?':
        return false;

      default:
        error++;
        print_usage();
        exit(EXIT_FAILURE);
    }

  }

  if(!(argc - optind)) {
    return false;
  }

  flags.filename = argv[argc - 1];

  return true;
}
