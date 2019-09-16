/**
 * This file contains implementations for the methods defined in the Simulation
 * class.
 *
 * You'll probably spend a lot of your time here.
 */

#include "simulation/simulation.h"
#include <iostream>
using namespace std;


Simulation::Simulation(const FlagOptions option) {
  // get filename from input
  this->filename = option.filename;
  // get strategy from input
  this->strategy = option.strategy;
  // get verbose
  this->is_verbose = option.verbose;
  // get max frame
  this->max_frames = option.max_frames;
}


void Simulation::run() {
  // TODO: implement me
  /**
   * get process id, address, and process from the input file 
   */
  size_t pid;
  string img;
  string address;

  // open file
  inputFile.open(filename);
  // Check open
  if(!inputFile){
    cerr<< "Error opening file: " << filename << endl;
    exit(EXIT_FAILURE);
  }
  // Check the number of process
  inputFile >> numProcess;
  // load pid and image of process into map
  for(unsigned int i = 0; i < numProcess; i++) {
    inputFile >> pid;
    inputFile >> img;
    map_process_id_file[pid] = img;
  }

  while(inputFile >> pid) {
    inputFile >> address;
    vector_pid_virtual_address.push_back(make_pair(pid,address));
  }

  /**
   * load process image 
   */
  for(auto i = map_process_id_file.begin(); i != map_process_id_file.end(); i++){
    ifstream inputFile(i->second);
    if(!inputFile){
      cerr << "Error opening file: " << i->second << endl;
      exit(EXIT_FAILURE);
    }
    processes[i->first] = Process::read_from_input(inputFile);
  }

  /**
   * Loop through the sim file
   */
  for(auto i = vector_pid_virtual_address.begin(); i != vector_pid_virtual_address.end(); i++){
    // get virtual memory address
    VirtualAddress va = VirtualAddress::from_string(i->first, i->second);
    // get the state of memory
    char state = perform_memory_access(va);
    // Check state, if wrong then out
    switch(state) {
      // SEGFAULT
      case -1:
        cerr << "SEGFAULT" << endl;
        exit(EXIT_FAILURE);
      // Correct
      case 0:
      case 1:
        break;
      // Unknow state
      default:
        cerr << "Unknown State" << endl;
        exit(EXIT_FAILURE);
    }
    // Verbose message print
    Process *p = processes[va.process_id];
    PhysicalAddress pa(p->page_table.rows[va.page].frame, va.offset);
    if(is_verbose) {
      cout << va << endl 
           << " -> " << (state ? "Page Fault" : "In Memory") << endl
           << " -> Physical Address: " << pa << endl
           << " -> RSS: " << p->page_table.get_present_page_count() << endl << endl;
    }
    // Update iteration
    iteration = iteration + 1;
  }

  /**
   * Print useful information
   */
  cout << "DONE!" << endl << endl;

  int totalMemoryAccess = 0;
  int totalPageFault = 0;
  for(auto i = processes.begin(); i != processes.end(); i++){
    totalMemoryAccess = totalMemoryAccess + i->second->memory_accesses;
    totalPageFault = totalPageFault + i->second->page_faults;
    cout << "Process " << i->first << ": ACCESSES: " << i->second->memory_accesses
         << "   FAULTS: " << i->second->page_faults << "   FAULT RATE: " << (double(i->second->page_faults * 100)/i->second->memory_accesses)
         << "   RSS: " << i->second->get_rss() << endl;
  }

  cout << "Total memory accesses:      " << totalMemoryAccess << endl;
  cout << "Total page faults:          " << totalPageFault << endl;
  cout << "Free frames remaining:      " << remainFrames << endl;

}


char Simulation::perform_memory_access(const VirtualAddress& address) {
  // get process by pid
  Process *p = processes[address.process_id];
  
  // Update memory access
  p->memory_accesses++;
  
  // if segfault
  if (!p->is_valid_page(address.page) || !p->pages[address.page]->is_valid_offset(address.offset))
        return -1;

  PageTable::Row &row = p->page_table.rows[address.page];
  if (!row.present) {
      handle_page_fault(address);
      return 1;
  }
  row.last_accessed_at = iteration;

  return 0;
}


void Simulation::handle_page_fault(const VirtualAddress& address) {
  // get process by address's pid
  Process *proc = processes[address.process_id];
  // get page 
  size_t page = address.page;
  // update page_faults
  proc->page_faults++;

  // if enough space in main memory and process's rss less than max_frame
  // schedule additional page
  if(remainFrames && proc->get_rss() < max_frames){
    proc->page_table.rows[page].present = true;
    if(proc->page_table.rows[page].loaded_at == -1) {
      int frame = mainMemory.size();
      // push PA into main memory
      mainMemory.push_back(PhysicalAddress(frame,0));
      proc->page_table.rows[page].frame = frame;
    }
    proc->page_table.rows[page].loaded_at = iteration;
    proc->page_table.rows[page].last_accessed_at = iteration;
    // decrement remain frame by 1
    remainFrames = remainFrames - 1;
  } else {
    // else use algorithm
    if(strategy == ReplacementStrategy::FIFO){
      size_t old_page = proc->page_table.get_oldest_page();
      size_t prev_frame = proc->page_table.rows[old_page].frame;

      // Swap out
      proc->page_table.rows[old_page].present = false;
      // Swap in
      proc->page_table.rows[page].present = true;
      proc->page_table.rows[page].frame = prev_frame;
      proc->page_table.rows[page].last_accessed_at = iteration;
      proc->page_table.rows[page].loaded_at = iteration;
    } else {
      size_t old_page = proc->page_table.get_least_recently_used_page();
      size_t prev_frame = proc->page_table.rows[old_page].frame;

      // Swap out
      proc->page_table.rows[old_page].present = false;
      // Swap in
      proc->page_table.rows[page].present = true;
      proc->page_table.rows[page].frame = prev_frame;
      proc->page_table.rows[page].last_accessed_at = iteration;
      proc->page_table.rows[page].loaded_at = iteration;
    }
  }
}
