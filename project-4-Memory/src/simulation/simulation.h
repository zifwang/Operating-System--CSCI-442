/**
 * This file contains the definition of the Simulation class.
 *
 * You're free to modify this class however you see fit. Add new methods to help
 * keep your code modular.
 */

#pragma once
#include "process/process.h"
#include "virtual_address/virtual_address.h"
#include "physical_address/physical_address.h"
#include "flag_parser/flag_parser.h"
#include <map>
#include <vector>
#include <fstream>
#include <string.h>
#include <iomanip>
#include <cstdlib>


/**
 * Class responsible for running the memory simulation.
 */
class Simulation {
// PUBLIC CONSTANTS
public:
  /**
   * The maximum number of frames in the simulated system (512).
   */
  static const size_t NUM_FRAMES = 1 << 9;    // 1 shift left 9 bits

// PUBLIC API METHODS
public:
  Simulation(const FlagOptions opt);
  /**
   * Runs the simulation.
   */
  void run();

// PRIVATE METHODS
private:

  /**
   * Performs a memory access for the given virtual address, translating it to
   * a physical address and loading the page into memory if needed. Returns the
   * byte at the given address.
   */
  char perform_memory_access(const VirtualAddress& address);

  /**
   * Handles a page fault, attempting to load the given page for the given
   * process into memory.
   */
  void handle_page_fault(const VirtualAddress& address);

// INSTANCE VARIABLES
private:
  // Flags
  // input file name
  std::string filename;
  // whether verbose
  bool is_verbose;
  // memory access algorithm
  ReplacementStrategy strategy;
  // the maximum number of frames
  size_t max_frames;
  // The input file
  std::ifstream inputFile;
  // number of processes
  size_t numProcess;
  // Number of remaining frame in memory
  size_t remainFrames = NUM_FRAMES;
  // iteration
  int iteration = 0;
  // A vector holds Physical Address of main memory
  std::vector<PhysicalAddress> mainMemory;
  // A map holds pid and process image, key pid
  std::map<size_t, std::string> map_process_id_file;
  // A vector holds a pid and virtual address pair, key pid
  std::vector<std::pair<size_t, std::string>> vector_pid_virtual_address;
  // A map of process, key pid, entry Process struct
  std::map<size_t, Process *> processes;
};
